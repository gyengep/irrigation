#include "IrrigationApplication.h"
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <thread>
#include <sys/unistd.h>
#include "Configuration.h"
#include "Exceptions/Exceptions.h"
#include "Hardware/GpioHandler.h"
#include "Logger/Logger.h"
#include "Logic/Program.h"
#include "Model/IrrigationDocument.h"
#include "ReaderWriter/XmlReader.h"
#include "ReaderWriter/XmlWriter.h"

#include <inttypes.h>


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

chrono::milliseconds getDiffBetweenSystemClockAndSteadyClock() {
	chrono::milliseconds steady = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch());
	chrono::milliseconds system = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());

	int64_t msSteady = chrono::milliseconds(steady).count();
	int64_t msSystem = chrono::milliseconds(system).count();

	return chrono::milliseconds(msSystem - msSteady);

}

chrono::milliseconds abs(const chrono::milliseconds& ms) {
	return chrono::milliseconds(abs(ms.count()));
}

void Application::start() {

	chrono::steady_clock::time_point monotonicTime = chrono::steady_clock::now();
	chrono::system_clock::time_point systemTime = chrono::system_clock::now();
	chrono::milliseconds expectedDiff = getDiffBetweenSystemClockAndSteadyClock();

	LOGGER.debug("Main loop started");

	while (!isTerminated) {

		monotonicTime += chrono::seconds(1);
		systemTime += chrono::seconds(1);

		this_thread::sleep_until(monotonicTime);

		chrono::milliseconds actualDiff = getDiffBetweenSystemClockAndSteadyClock();
		chrono::milliseconds diffOfDiff = actualDiff - expectedDiff;

		if (abs(diffOfDiff) > chrono::milliseconds(100)) {
			ostringstream o;
			o << "Update period failure!";

			if (abs(diffOfDiff) > chrono::seconds(1)) {
				time_t previousTime = chrono::system_clock::to_time_t(systemTime);
				time_t currentTime = chrono::system_clock::to_time_t(chrono::system_clock::now());

				o << " time is changed";
				o << " from " << put_time(localtime(&previousTime), "%Y.%m.%d %H:%M:%S");
				o << " to " << put_time(localtime(&currentTime), "%Y.%m.%d %H:%M:%S");
			} else {
				o << " different is: " << diffOfDiff.count() << " ms";
			}

			LOGGER.warning(o.str().c_str());

			monotonicTime = chrono::steady_clock::now();
			systemTime = chrono::system_clock::now();
			expectedDiff = getDiffBetweenSystemClockAndSteadyClock();
		}

		document->on1SecTimer(chrono::system_clock::to_time_t(systemTime));
	}

	LOGGER.debug("Main loop finished");
}

void Application::stop() {
	//saveDocument(Configuration::getInstance().getConfigFileName());
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

//void Application::saveDocument(const string& fileName) const {
//	const DocumentDTO documentDTO = document->getDocumentDTO();
//	const string xml = XmlWriter().save(documentDTO);
//
//	try {
//		writeFile(fileName, xml);
//		LOGGER.debug("Configuration successfully saved.");
//	} catch (const IOException& e) {
//		LOGGER.warning("Configuration file saving failed. %s", e.what());
//	}
//}
//
//void Application::writeFile(const string& fileName, const string& text) {
//	ofstream ofs;
//
//	ofs.open(fileName, ofstream::out | ofstream::trunc);
//	if (ofs.fail()) {
//		throw IOException(errno);
//	}
//
//	ofs << text;
//	if (ofs.fail()) {
//		throw IOException(errno);
//	}
//
//	ofs.close();
//	if (ofs.fail()) {
//		throw IOException(errno);
//	}
//}
