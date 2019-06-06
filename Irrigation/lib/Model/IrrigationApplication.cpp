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
	virtual std::shared_ptr<DtoWriter> create() override {
		return make_shared<XmlWriter>();
	}
};

class FileWriterFactory : public DocumentSaver::FileWriterFactory {
	const std::string fileName;

public:
	FileWriterFactory(const std::string& fileName) : fileName(fileName) {}
	virtual ~FileWriterFactory() = default;
	virtual std::shared_ptr<FileWriter> create() override {
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
			Configuration::getInstance().getTemperatureCacheFileName(),
			Configuration::getInstance().getTemperatureHistoryFileName()
		);

	} catch (const exception& e) {
		throw_with_nested(runtime_error("Can't initialize temperature sensor"));
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

void IrrigationApplication::onInitialize() {
	LOGGER.info("Irrigation System %s", getVersion().c_str());
	LOGGER.debug("Irrigation System starting ...");

	initGpio();
	initTemperatureSensor();
	initDocument();

	documentSaver->start();
	LOGGER.info("Irrigation System started");
}

void IrrigationApplication::onTerminate() {
	LOGGER.debug("Irrigation System stopping ... ");

	documentSaver->stop();

	try {
		documentSaver->saveIfModified();
		LOGGER.debug("Configuration successfully saved.");

	} catch (const exception& e) {
		throw_with_nested(runtime_error("Can't save configuration"));
	}

	documentSaver.reset();
	irrigationDocument.reset();

	LOGGER.info("Irrigation System stopped");
}
