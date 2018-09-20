#include "IrrigationApplication.h"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <thread>
#include "Configuration.h"
#include "Exceptions/Exceptions.h"
#include "Hardware/GpioHandler.h"
#include "Logger/Logger.h"
#include "Model/IrrigationDocument.h"
#include "ReaderWriter/XmlReader.h"
#include "ReaderWriter/XmlWriter.h"
#include "Views/TimerView/TimerView.h"


using namespace std;


mutex IrrigationApplication::createMutex;
unique_ptr<IrrigationApplication> IrrigationApplication::instance;


IrrigationApplication& IrrigationApplication::getInstance() {
	if (nullptr == instance) {
		lock_guard<mutex> lock(createMutex);

		if (nullptr == instance) {
			instance.reset(new IrrigationApplication());
		}
	}

	return *instance.get();
}

void IrrigationApplication::initGpio() {
	try {
		GpioHandler::init();
	} catch (const exception& e) {
		throw_with_nested(runtime_error("Can't initialize valves"));
	}
}

void IrrigationApplication::initDocument() {
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

	document->addView(new TimerView(*document.get()));
}

void IrrigationApplication::onInitialize() {
	LOGGER.debug("Irrigation System starting ...");

	initGpio();
	initDocument();

	LOGGER.info("Irrigation System started");
}

void IrrigationApplication::onTerminate() {
	LOGGER.debug("Irrigation System stopping ... ");

	//saveDocument(Configuration::getInstance().getConfigFileName());
	document.reset();

	LOGGER.info("Irrigation System stopped");
}

string IrrigationApplication::readFile(const string& fileName) {
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

//void IrrigationApplication::saveDocument(const string& fileName) const {
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
//void IrrigationApplication::writeFile(const string& fileName, const string& text) {
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
