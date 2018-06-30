#include "IrrigationApplication.h"
#include <chrono>
#include <fstream>
#include <mutex>
#include <sys/unistd.h>
#include "Configuration.h"
#include "Exceptions/Exceptions.h"
#include "Hardware/GpioHandler.h"
#include "Logger/Logger.h"
#include "Logic/Program.h"
#include "Model/IrrigationDocument.h"
#include "ReaderWriter/XmlReader.h"
#include "ReaderWriter/XmlWriter.h"


using namespace std;


mutex Application::createMutex;
unique_ptr<Application> Application::instance;


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

void Application::initGpio() {
	try {
		GpioHandler::init();
	} catch (const exception& e) {
		throw_with_nested(runtime_error("Can't initialize valves"));
	}
}

void Application::initDocument() {
	document.reset(new IrrigationDocument());

	try {
		LOGGER.debug("Loading configuration...");

		const string xml = readFile(Configuration::getInstance().getConfigFileName());
		const DocumentDTO documentDTO = XmlReader().loadDocument(xml);
		document->updateFromDTO(documentDTO);

		LOGGER.debug("Configuration is successfully loaded");
	} catch (const FileNotFoundException& e) {
		LOGGER.debug("Configuration file not found. Default configuration is loaded.");
	} catch (const IOException& e) {
		throw_with_nested(runtime_error("Can't open configuration file"));
	} catch (const XMLParseException& e) {
		throw_with_nested(runtime_error("Can't parse configuration file"));
	} catch (const exception& e) {
		throw_with_nested(runtime_error("Can't initialize document"));
	}
}

void Application::init() {
	LOGGER.info("Irrigation System started");

	initGpio();
	initDocument();
}

void Application::start() {

	auto updateTimePoint = chrono::steady_clock::now();
	time_t lastTime = 0;

	LOGGER.debug("Main loop started");

	while (!isTerminated) {
		time_t currentTime = getTime();
		int diffTime = currentTime - lastTime;

		if ((diffTime != 1) && (lastTime != 0)) {
			LOGGER.warning("Update period failure! different is: %d", diffTime);
		}

		lastTime = currentTime;
		document->on1SecTimer(currentTime);

		updateTimePoint += chrono::seconds(1);
		this_thread::sleep_until(updateTimePoint);
	}

	LOGGER.debug("Main loop finished");
}

void Application::stop() {
	saveDocument(Configuration::getInstance().getConfigFileName());
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

string Application::readFile(const string& fileName) {
	ifstream ifs(fileName);

	if (ifs.fail()) {
		if (ENOENT == errno) {
			throw FileNotFoundException();
		} else {
			throw IOException(errno);
		}
	}

	string buffer(
		(istreambuf_iterator<char>(ifs)),
		(istreambuf_iterator<char>())
		);

	if (ifs.fail()) {
		throw IOException(errno);
	}

	ifs.close();

	if (ifs.fail()) {
		throw IOException(errno);
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
		throw IOException(errno);
	}

	ofs << text;
	if (ofs.fail()) {
		throw IOException(errno);
	}

	ofs.close();
	if (ofs.fail()) {
		throw IOException(errno);
	}
}
