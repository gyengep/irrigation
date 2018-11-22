#include "IrrigationApplication.h"
#include "Configuration.h"
#include <stdexcept>
#include <thread>
#include "DtoReaderWriter/XMLParseException.h"
#include "Exceptions/Exceptions.h"
#include "Hardware/Valves/GpioHandler.h"
#include "Logger/Logger.h"
#include "Model/IrrigationDocument.h"
#include "Views/TimerView/TimerView.h"

using namespace std;


IrrigationApplication::IrrigationApplication() {
}

IrrigationApplication::~IrrigationApplication() {
}

IrrigationApplication& IrrigationApplication::getInstance() {
	static IrrigationApplication instance;
	return instance;
}

string IrrigationApplication::getVersion() {
	return version_string;
}

void IrrigationApplication::initGpio() {
	try {
		GpioHandler::init();
	} catch (const exception& e) {
		throw_with_nested(runtime_error("Can't initialize valves"));
	}
}

void IrrigationApplication::initDocument() {
	document = IrrigationDocument::Builder().build();

	try {
		document->load(Configuration::getInstance().getConfigFileName());

	} catch (const FileNotFoundException& e) {
		LOGGER.debug("Configuration file not found. Default configuration is loaded.");
	} catch (const IOException& e) {
		throw_with_nested(runtime_error("Can't open configuration file"));
	} catch (const XMLParseException& e) {
		throw_with_nested(runtime_error("Can't parse configuration file"));
	} catch (const exception& e) {
		throw_with_nested(runtime_error("Can't initialize document"));
	}

	document->addView(unique_ptr<View>(new TimerView(*document.get())));
}

void IrrigationApplication::onInitialize() {
	LOGGER.info("Irrigation System %s", getVersion().c_str());
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
