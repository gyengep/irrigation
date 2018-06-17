#include "IrrigationApplication.h"
#include "Exceptions/IOException.h"
#include "Exceptions/InvalidConfigFileException.h"
#include "Exceptions/ConfigFileOpenException.h"
#include <fstream>
#include <chrono>
#include <mutex>
#include <cstring>
#include <sys/unistd.h>
#include "Hardware/Valves.h"
#include "Logger/Logger.h"
#include "Logic/Program.h"
#include "Model/IrrigationDocument.h"
#include "ReaderWriter/XmlReader.h"
#include "ReaderWriter/XmlWriter.h"


using namespace std;


mutex Application::createMutex;
unique_ptr<Application> Application::instance;

const string Application::configFileName = "/tmp/irrigation.xml";
const string Application::logFileName = "/tmp/irrigation.log";
const Logger::Level Application::logLevel = Logger::Level::TRACE;


Application& Application::getInstance() {
	if (nullptr == instance) {
		lock_guard<mutex> lock(createMutex);

		if (nullptr == instance) {
			instance.reset(new Application());
		}
	}

	return *instance.get();
}

Application::Application() :
	isTerminated(false)
{
}

Application::~Application() {
}

void Application::init() {

	LOGGER.setOutput(new ofstream(logFileName, ofstream::out | ofstream::app));
	LOGGER.setLevel(logLevel);

	LOGGER.info("Irrigation System started");
	Valves::init();
	
	document.reset(new IrrigationDocument());
	loadDocument(configFileName);
}

void Application::start() {

	auto updateTimePoint = chrono::steady_clock::now();
	time_t lastTime = 0;

	while (!isTerminated) {
		time_t currentTime = getTime();

		if ((currentTime != (lastTime + 1)) && (lastTime != 0)) {
			LOGGER.warning("Update period failure! different is: %d", (currentTime - lastTime));
		}

		lastTime = currentTime;
		document->on1SecTimer(currentTime);

		updateTimePoint += chrono::seconds(1);
		this_thread::sleep_until(updateTimePoint);
	}

}

void Application::stop() {
	saveDocument(configFileName);
	document.reset();

	LOGGER.info("Irrigation System stopped");
}

void Application::terminate() { 
	LOGGER.debug("Terminate called");
	isTerminated = true;
}

time_t Application::getTime() const {
	return time(nullptr);
}

void Application::loadDocument(const string& fileName) {
	DocumentDTO documentDTO;

	try {
		string xml = readFile(configFileName);
		XmlReader().load(documentDTO, xml);
		LOGGER.debug("Configuration successfully loaded.");
	} catch (FileNotFoundException& e) {
		LOGGER.debug("Configuration file not found. Default configuration is loaded.");
	} catch (IOException& e) {
		throw ConfigFileOpenException(string("Can't open configuration file. ") + e.what());
	} catch (XmlReaderException& e) {
		throw InvalidConfigFileException(string("Configuration file parsing failed. ") + e.what());
	}

	document->updateFromDTO(documentDTO);

	if (LOGGER.isLoggable(Logger::DEBUG)) {
		for (auto idProgramPair : document->getPrograms()) {
			LOGGER.debug("Program[%u]=%s", (unsigned)idProgramPair.first, idProgramPair.second->toString().c_str());
		}
	}
}

string Application::readFile(const string& fileName) {
	ifstream ifs(fileName);

	if (ifs.fail()) {
		if (ENOENT == errno) {
			throw FileNotFoundException("File not found.");
		} else {
			throw IOException(strerror(errno));
		}
	}

	string buffer(
		(std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>())
		);

	if (ifs.fail()) {
		throw IOException(strerror(errno));
	}

	ifs.close();
	if (ifs.fail()) {
		throw IOException(strerror(errno));
	}

	return buffer;
}

void Application::saveDocument(const string& fileName) const {
	const DocumentDTO documentDTO = document->getDocumentDTO();
	const string xml = XmlWriter().save(documentDTO);

	try {
		writeFile(fileName, xml);
		LOGGER.debug("Configuration successfully saved.");
	} catch (const IOException& e) {
		LOGGER.warning("Configuration file saving failed. %s", e.what());
	}
}

void Application::writeFile(const string& fileName, const string& text) {
	ofstream ofs;

	ofs.open(fileName, ofstream::out | ofstream::trunc);
	if (ofs.fail()) {
		throw IOException(strerror(errno));
	}

	ofs << text;
	if (ofs.fail()) {
		throw IOException(strerror(errno));
	}

	ofs.close();
	if (ofs.fail()) {
		throw IOException(strerror(errno));
	}
}
