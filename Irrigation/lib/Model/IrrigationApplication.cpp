#include "IrrigationApplication.h"
#include "Configuration.h"
#include "DocumentSaver.h"
#include "IrrigationDocumentImpl.h"
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

using namespace std;


class XmlWriterFactory : public DocumentSaver::DtoWriterFactory {
public:
	virtual ~XmlWriterFactory() = default;
	virtual shared_ptr<DtoWriter> create() override {
		return make_shared<XmlWriter>();
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
	emailHandler = std::make_shared<EmailHandler>(
			Email::Contact("Irrigation System", "irrigation.gyengep@gmail.com"),
			Email::Contact("Gyenge Peter", "gyengep@gmail.com"),
			EmailSender::create(),
			std::vector<std::chrono::milliseconds> {
					chrono::minutes(1), chrono::minutes(2), chrono::minutes(5),
					chrono::minutes(15), chrono::minutes(30), chrono::minutes(60)
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

	} catch (const exception& e) {
		throw_with_nested(runtime_error("Can't initialize temperature module"));
	}
}

void IrrigationApplication::uninitTemperature() {
	try {
		temperatureHandler.reset();
	} catch (const exception& e) {
		LOGGER.warning("An error during uninitialize temperature module", e);
	}
}

void IrrigationApplication::initDocument() {

	irrigationDocument = std::make_shared<IrrigationDocumentImpl>(
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

	documentSaver.reset(new DocumentSaver(
		irrigationDocument,
		make_shared<XmlWriterFactory>(),
		make_shared<FileWriterFactoryImpl>(Configuration::getInstance().getConfigFileName())
	));

	try {
		LOGGER.debug("Loading configuration...");

		if (true) {
			documentSaver->load(
				make_shared<XmlReader>(),
				make_shared<FileReaderImpl>(Configuration::getInstance().getConfigFileName())
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
			temperatureHandler->getCurrentTemperature(),
			temperatureHandler->getTemperatureForecast(),
			temperatureHandler->getTemperatureHistory(),
			shutdownManager,
			std::make_shared<FileWriterFactoryImpl>(Configuration::getInstance().getAccessLogFileName()),
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
