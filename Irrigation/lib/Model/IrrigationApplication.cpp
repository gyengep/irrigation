#include "IrrigationApplication.h"
#include "Configuration.h"
#include "IrrigationDocumentImpl.h"
#include "IrrigationDocumentLoaderImpl.h"
#include "IrrigationDocumentLoaderMyDefaults.h"
#include "IrrigationDocumentSaverImpl.h"
#include "DtoReaderWriter/XMLParseException.h"
#include "DtoReaderWriter/XmlReader.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "Exceptions/Exceptions.h"
#include "Hardware/Valves/GpioValve.h"
#include "Hardware/Valves/ZoneHandlerImpl.h"
#include "Logger/Logger.h"
#include "Logic/ProgramImpl.h"
#include "Logic/RunTimeImpl.h"
#include "Logic/StartTimeImpl.h"
#include "Logic/ProgramContainerImpl.h"
#include "Logic/RunTimeContainerImpl.h"
#include "Logic/StartTimeContainerImpl.h"
#include "Logic/SchedulerContainerImpl.h"
#include "Logic/WateringControllerImpl.h"
#include "Schedulers/EveryDaySchedulerImpl.h"
#include "Schedulers/HotWeatherSchedulerImpl.h"
#include "Schedulers/TemperatureDependentSchedulerImpl.h"
#include "Schedulers/WeeklySchedulerImpl.h"
#include "Utils/FileReaderImpl.h"
#include "Utils/FileWriterImpl.h"
#include "Views/RestView/RestView.h"
#include "Views/TimerView/TimerView.h"
#include <stdexcept>


std::shared_ptr<IrrigationDocument::Loader> IrrigationDocument::Loader::create() {
	if (true) {
		return std::make_shared<IrrigationDocumentLoaderImpl>(
			std::make_shared<XmlReader>(),
			std::make_shared<FileReaderImpl>(Configuration::getInstance().getConfigFileName())
		);
	} else {
		return std::make_shared<IrrigationDocumentLoaderMyDefaults>();
	}
}

std::shared_ptr<IrrigationDocument::Saver> IrrigationDocument::Saver::create() {
	return std::make_shared<IrrigationDocumentSaverImpl>(
		std::make_shared<XmlWriter>(),
		std::make_shared<FileWriterImplFactory>(Configuration::getInstance().getConfigFileName())
	);
}

///////////////////////////////////////////////////////////////////////////////

IrrigationApplication::IrrigationApplication() {
}

IrrigationApplication::~IrrigationApplication() {
}

IrrigationApplication& IrrigationApplication::getInstance() {
	static IrrigationApplication instance;
	return instance;
}

std::string IrrigationApplication::getVersion() {
	return version_string;
}

void IrrigationApplication::initEmail() {
	emailHandler = std::make_shared<EmailHandler>(
			Email::Contact("Irrigation System", "irrigation.gyengep@gmail.com"),
			Email::Contact("Gyenge Peter", "gyengep@gmail.com"),
			EmailSender::create(),
			std::vector<std::chrono::milliseconds> {
					std::chrono::minutes(1), std::chrono::minutes(2), std::chrono::minutes(5),
					std::chrono::minutes(15), std::chrono::minutes(30), std::chrono::minutes(60)
				}
		);

	emailHandler->enableTopic(EmailTopic::WATERING_START);
	emailHandler->enableTopic(EmailTopic::WATERING_SKIP);
	emailHandler->enableTopic(EmailTopic::SYSTEM_STARTED);
	emailHandler->enableTopic(EmailTopic::SYSTEM_STOPPED);
	emailHandler->start();

	emailHandler->send(EmailTopic::SYSTEM_STARTED, "System started");
}

void IrrigationApplication::uninitEmail() {
	emailHandler->send(EmailTopic::SYSTEM_STOPPED, "System stopped");
	emailHandler->stop();
}

void IrrigationApplication::initGpio() {
	try {
		GpioValve::init();
	} catch (const std::exception& e) {
		std::throw_with_nested(std::runtime_error("Can't initialize valves"));
	}
}

void IrrigationApplication::initShutdownManager() {
	shutdownManager = std::make_shared<ShutdownManagerImpl>();
}

void IrrigationApplication::uninitShutdownManager() {
	shutdownManager.reset();
}

void IrrigationApplication::initTemperature() {
	try {
		temperatureHandler = std::make_shared<TemperatureHandler>(
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

	} catch (const std::exception& e) {
		std::throw_with_nested(std::runtime_error("Can't initialize temperature module"));
	}
}

void IrrigationApplication::uninitTemperature() {
	try {
		temperatureHandler.reset();
	} catch (const std::exception& e) {
		LOGGER.warning("An error during uninitialize temperature module", e);
	}
}

void IrrigationApplication::initDocument() {

	irrigationDocument = std::make_shared<IrrigationDocumentImpl>(
			IrrigationDocumentImpl::Loader::create(),
			IrrigationDocumentImpl::Saver::create(),
			std::make_shared<ProgramContainerImpl>(
				std::make_shared<ProgramImplFactory>(
					std::make_shared<SchedulerContainerImplFactory>(
						std::make_shared<EveryDaySchedulerImplFactory>(),
						std::make_shared<HotWeatherSchedulerImplFactory>(
							temperatureHandler->getTemperatureHistory()
						),
						std::make_shared<TemperatureDependentSchedulerImplFactory>(
							temperatureHandler->getTemperatureForecast(),
							temperatureHandler->getTemperatureHistory()
						),
						std::make_shared<WeeklySchedulerImplFactory>()
					),
					std::make_shared<RunTimeContainerImplFactory>(
						std::make_shared<RunTimeImplFactory>()
					),
					std::make_shared<StartTimeContainerImplFactory>(
						std::make_shared<StartTimeImplFactory>()
					)
				)
			),
			std::make_shared<WateringControllerImpl>(
				std::make_shared<ZoneHandlerImpl>(GpioValve::getValves())
			),
			emailHandler
	);

	try {
		LOGGER.debug("Loading configuration...");
		irrigationDocument->load();
	} catch (const FileNotFoundException& e) {
		LOGGER.debug("Configuration file not found. Default configuration is loaded.");
	} catch (const IOException& e) {
		std::throw_with_nested(std::runtime_error("Can't open configuration file"));
	} catch (const XMLParseException& e) {
		std::throw_with_nested(std::runtime_error("Can't parse configuration file"));
	} catch (const std::exception& e) {
		std::throw_with_nested(std::runtime_error("Can't initialize document"));
	}

	irrigationDocument->loadState();
	irrigationDocument->addView(std::unique_ptr<View>(new TimerView(*irrigationDocument)));
	irrigationDocument->addView(std::unique_ptr<View>(new RestView(*irrigationDocument,
			Configuration::getInstance().getRestPort(),
			temperatureHandler->getCurrentTemperature(),
			temperatureHandler->getTemperatureForecast(),
			temperatureHandler->getTemperatureHistory(),
			shutdownManager,
			std::make_shared<FileWriterImplFactory>(Configuration::getInstance().getAccessLogFileName()),
			Configuration::getInstance().getResourceDirectory()
		)));

	periodicDocumentSaver.reset(new PeriodicDocumentSaver(irrigationDocument));
	periodicDocumentSaver->startTimer();
}

void IrrigationApplication::uninitDocument() {
	periodicDocumentSaver->stopTimer();
	periodicDocumentSaver.reset();

	try {
		if (irrigationDocument->isModified()) {
			irrigationDocument->save();
		}
	} catch (const std::exception& e) {
		std::throw_with_nested(std::runtime_error("Can't save configuration"));
	}

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
