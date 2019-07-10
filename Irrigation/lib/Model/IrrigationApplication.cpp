#include "IrrigationApplication.h"
#include "Configuration.h"
#include "DocumentSaver.h"
#include "DtoReaderWriter/XMLParseException.h"
#include "DtoReaderWriter/XmlReader.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "Exceptions/Exceptions.h"
#include "Hardware/Temperature/Temperature.h"
#include "Hardware/Valves/GpioHandler.h"
#include "Logger/Logger.h"
#include "Model/IrrigationDocument.h"
#include "Views/RestView/RestView.h"
#include "Views/TimerView/TimerView.h"
#include "Utils/FileReaderWriterImpl.h"
#include <stdexcept>

using namespace std;


class XmlWriterFactory : public DocumentSaver::DtoWriterFactory {
public:
	virtual ~XmlWriterFactory() = default;
	virtual shared_ptr<DtoWriter> create() override {
		return make_shared<XmlWriter>();
	}
};

class FileWriterFactory : public DocumentSaver::FileWriterFactory {
	const string fileName;

public:
	FileWriterFactory(const string& fileName) : fileName(fileName) {}
	virtual ~FileWriterFactory() = default;
	virtual shared_ptr<FileWriter> create() override {
		return make_shared<FileWriterImpl>(fileName);
	}
};

///////////////////////////////////////////////////////////////////////////////

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

void IrrigationApplication::initTemperatureSensor() {
	try {
		Temperature::init(
			Configuration::getInstance().getTemperatureSensorUpdatePeriod(),
			Configuration::getInstance().getTemperatureCacheFileName(),
			Configuration::getInstance().getTemperatureCacheLength(),
			Configuration::getInstance().getTemperatureHistoryFileName(),
			Configuration::getInstance().getTemperatureHistoryPeriod(),
			Configuration::getInstance().getTemperatureForecastUpdatePeriod()
		);

	} catch (const exception& e) {
		throw_with_nested(runtime_error("Can't initialize temperature sensor"));
	}
}

void IrrigationApplication::uninitTemperatureSensor() {
	try {
		Temperature::uninit();
	} catch (const exception& e) {
		LOGGER.warning("An error during uninitialize temperature modul", e);
	}
}

void IrrigationApplication::initDocument() {
	irrigationDocument = IrrigationDocument::Builder().build();

	documentSaver.reset(new DocumentSaver(
		irrigationDocument,
		make_shared<XmlWriterFactory>(),
		make_shared<FileWriterFactory>(Configuration::getInstance().getConfigFileName())
	));

	try {
		LOGGER.debug("Loading configuration...");

		documentSaver->load(
			make_shared<XmlReader>(),
			make_shared<FileReaderImpl>(Configuration::getInstance().getConfigFileName())
		);

		documentSaver->startTimer();

	} catch (const FileNotFoundException& e) {
		LOGGER.debug("Configuration file not found. Default configuration is loaded.");
	} catch (const IOException& e) {
		throw_with_nested(runtime_error("Can't open configuration file"));
	} catch (const XMLParseException& e) {
		throw_with_nested(runtime_error("Can't parse configuration file"));
	} catch (const exception& e) {
		throw_with_nested(runtime_error("Can't initialize document"));
	}

	irrigationDocument->addView(unique_ptr<View>(new TimerView(*irrigationDocument)));
	irrigationDocument->addView(unique_ptr<View>(new RestView(*irrigationDocument, Configuration::getInstance().getRestPort())));
}

void IrrigationApplication::uninitDocument() {
	documentSaver->stopTimer();

	try {
		documentSaver->saveIfModified();
	} catch (const exception& e) {
		throw_with_nested(runtime_error("Can't save configuration"));
	}

	documentSaver.reset();
	irrigationDocument.reset();
}

void IrrigationApplication::onInitialize() {
	LOGGER.info("Irrigation System %s", getVersion().c_str());
	LOGGER.debug("Irrigation System starting ...");

	initGpio();
	initTemperatureSensor();
	initDocument();

	LOGGER.info("Irrigation System started");
}

void IrrigationApplication::onTerminate() {
	LOGGER.debug("Irrigation System stopping ... ");

	uninitDocument();
	uninitTemperatureSensor();

	LOGGER.info("Irrigation System stopped");
}
