#include "IrrigationApplication.h"
#include "Configuration.h"
#include <stdexcept>
#include "DtoReaderWriter/XMLParseException.h"
#include "Exceptions/Exceptions.h"
#include "Hardware/GpioHandler.h"
#include "Logger/Logger.h"
#include "Model/IrrigationDocument.h"
#include "Views/TimerView/TimerView.h"

#define VERSION "v0.1.1"
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

std::string IrrigationApplication::getVersion() {
	return VERSION;
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

	document->addView(new TimerView(*document.get()));
}

void IrrigationApplication::onInitialize() {
	LOGGER.debug("Irrigation System " VERSION "starting ...");

	initGpio();
	initDocument();

	LOGGER.info("Irrigation System " VERSION " started");
}

void IrrigationApplication::onTerminate() {
	LOGGER.debug("Irrigation System stopping ... ");

	//saveDocument(Configuration::getInstance().getConfigFileName());
	document.reset();

	LOGGER.info("Irrigation System stopped");
}
