#include "IrrigationApplication.h"
#include "Configuration.h"
#include "DocumentSaver.h"
#include "IrrigationDocumentImpl.h"
#include "DtoReaderWriter/XMLParseException.h"
#include "DtoReaderWriter/XmlReader.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "Email/EmailHandler.h"
#include "Exceptions/Exceptions.h"
#include "Hardware/Valves/GpioValve.h"
#include "Logger/Logger.h"
#include "Logic/ProgramImplBuilder.h"
#include "Utils/FileReaderWriterImpl.h"
#include "Views/RestView/RestView.h"
#include "Views/TimerView/TimerView.h"
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

void IrrigationApplication::initEmail() {
	EmailHandler::init(
			Email::Contact("Irrigation System", "irrigation.gyengep@gmail.com"),
			Email::Contact("Gyenge Peter", "gyengep@gmail.com"),
			EmailSender::create(),
			std::vector<std::chrono::milliseconds> {
					chrono::minutes(1), chrono::minutes(2), chrono::minutes(5),
					chrono::minutes(15), chrono::minutes(30), chrono::minutes(60)
				}
		);

	EMAIL.enableTopic(EmailTopic::WATERING_START);
	EMAIL.enableTopic(EmailTopic::WATERING_SKIP);
	EMAIL.enableTopic(EmailTopic::SYSTEM_STARTED);
	EMAIL.enableTopic(EmailTopic::SYSTEM_STOPPED);

	EMAIL.send(EmailTopic::SYSTEM_STARTED, "System started");
}

void IrrigationApplication::uninitEmail() {
	EMAIL.send(EmailTopic::SYSTEM_STOPPED, "System stopped");
	EmailHandler::uninit();
}

void IrrigationApplication::initGpio() {
	try {
		GpioValve::init();
	} catch (const exception& e) {
		throw_with_nested(runtime_error("Can't initialize valves"));
	}
}

void IrrigationApplication::initShutdownManager() {
	shutdownManager = make_shared<ShutdownManagerImpl>();
}

void IrrigationApplication::uninitShutdownManager() {
	shutdownManager.reset();
}

void IrrigationApplication::initTemperature() {
	try {
		TemperatureHandler::getInstance().init(
				TemperatureHandler::CurrentTemperatureProperties(
						Configuration::getInstance().getCurrentTemperatureUpdatePeriod(),
						std::vector<std::chrono::milliseconds> {
							std::chrono::minutes(1), std::chrono::minutes(2), std::chrono::minutes(5),
							std::chrono::minutes(15), std::chrono::minutes(30), std::chrono::minutes(60)
						}
					),
				TemperatureHandler::TemperatureForecastProperties(
						Configuration::getInstance().getTemperatureForecastUpdatePeriod(),
						std::vector<std::chrono::milliseconds> {
							std::chrono::minutes(1), std::chrono::minutes(2), std::chrono::minutes(5),
							std::chrono::minutes(15), std::chrono::minutes(30), std::chrono::minutes(60)
						}
					),
				TemperatureHandler::TemperatureHistoryProperties(
						Configuration::getInstance().getTemperatureCacheLength(),
						Configuration::getInstance().getTemperatureCacheFileName()
					),
				TemperatureHandler::TemperatureHistoryLoggerProperties(
						Configuration::getInstance().getTemperatureHistoryPeriod(),
						Configuration::getInstance().getTemperatureHistoryFileName()
					)
			);

	} catch (const exception& e) {
		throw_with_nested(runtime_error("Can't initialize temperature module"));
	}
}

void IrrigationApplication::uninitTemperature() {
	try {
		TemperatureHandler::getInstance().uninit();
	} catch (const exception& e) {
		LOGGER.warning("An error during uninitialize temperature module", e);
	}
}

void IrrigationApplication::initDocument() {
	irrigationDocument = IrrigationDocumentImpl::Builder().build();

	documentSaver.reset(new DocumentSaver(
		irrigationDocument,
		make_shared<XmlWriterFactory>(),
		make_shared<FileWriterFactory>(Configuration::getInstance().getConfigFileName())
	));

	try {
		LOGGER.debug("Loading configuration...");

		if (true) {
			documentSaver->load(
				make_shared<XmlReader>(),
				make_shared<FileReaderImpl>(Configuration::getInstance().getConfigFileName()),
				ProgramImplFactory::Builder().build()
			);
		} else {
			setMyDefaults();
		}

		irrigationDocument->loadState();
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
	irrigationDocument->addView(unique_ptr<View>(new RestView(*irrigationDocument,
			Configuration::getInstance().getRestPort(),
			TemperatureHandler::getInstance().getCurrentTemperature(),
			TemperatureHandler::getInstance().getTemperatureForecast(),
			TemperatureHandler::getInstance().getTemperatureHistory(),
			shutdownManager,
			Configuration::getInstance().getResourceDirectory()
		)));
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

	initEmail();
	initGpio();
	initTemperature();
	initShutdownManager();
	initDocument();

	LOGGER.info("Irrigation System started");
}

void IrrigationApplication::onTerminate() {
	LOGGER.debug("Irrigation System stopping ... ");

	uninitDocument();
	uninitTemperature();
	uninitEmail();
	uninitShutdownManager();

	LOGGER.info("Irrigation System stopped");
}
