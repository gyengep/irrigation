#include "IrrigationApplication.h"
#include "Configuration.h"
#include "DocumentSaver.h"
#include "DtoReaderWriter/XMLParseException.h"
#include "DtoReaderWriter/XmlReader.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "Email/EmailHandler.h"
#include "Exceptions/Exceptions.h"
#include "Hardware/Valves/GpioValve.h"
#include "Hardware/Valves/ZoneHandlerImpl.h"
#include "Logger/Logger.h"
#include "Logic/Program.h"
#include "Logic/ProgramContainer.h"
#include "Logic/RunTimeImpl.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTimeImpl.h"
#include "Logic/StartTimeContainer.h"
#include "Logic/WateringController.h"
#include "Model/IrrigationDocument.h"
#include "Schedulers/TemperatureDependentSchedulerImpl.h"
#include "Schedulers/HotWeatherSchedulerImpl.h"
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
			EmailSender::create(EmailSender::Properties("smtps://smtp.gmail.com:465", "irrigation.gyengep", "hFlTL4931c")),
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
	irrigationDocument = std::make_shared<IrrigationDocument>(
		std::make_shared<ProgramContainer>(),
		std::make_shared<WateringController>(
			std::make_shared<ZoneHandlerImpl>(GpioValve::getValves())
		)
	);

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

void IrrigationApplication::setMyDefaults() {
	irrigationDocument->getPrograms().insert(
		IdType(),
		Program::Builder().
			setName("fulocsolas").
			setEnabled(true).
			setAdjustment(100).
			setSchedulerType(SchedulerType::TEMPERATURE_DEPENDENT).
			setTemperatureDependentScheduler(make_shared<TemperatureDependentSchedulerImpl>(
				TemperatureHandler::getInstance().getTemperatureForecast(),
				TemperatureHandler::getInstance().getTemperatureHistory(),
				0.75f,
				50, 0,
				100
			)).
			setRunTimeContainer(shared_ptr<RunTimeContainer>(new RunTimeContainer {
				std::make_shared<RunTimeImpl>(chrono::minutes(26)),
				std::make_shared<RunTimeImpl>(chrono::minutes(38)),
				std::make_shared<RunTimeImpl>(chrono::minutes(32)),
				std::make_shared<RunTimeImpl>(chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(chrono::minutes(0))
			})).
			setStartTimeContainer(shared_ptr<StartTimeContainer>(new StartTimeContainer {
				{ IdType(), make_shared<StartTimeImpl>(4, 0) }
			})).
			build()
		);

	irrigationDocument->getPrograms().insert(
		IdType(),
		Program::Builder().
			setName("buxus").
			setEnabled(true).
			setAdjustment(100).
			setSchedulerType(SchedulerType::TEMPERATURE_DEPENDENT).
			setTemperatureDependentScheduler(make_shared<TemperatureDependentSchedulerImpl>(
				TemperatureHandler::getInstance().getTemperatureForecast(),
				TemperatureHandler::getInstance().getTemperatureHistory(),
				1.0f,
				100, 0,
				0
			)).
			setRunTimeContainer(shared_ptr<RunTimeContainer>(new RunTimeContainer {
				std::make_shared<RunTimeImpl>(chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(chrono::minutes(15)),
				std::make_shared<RunTimeImpl>(chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(chrono::minutes(0))
			})).
			setStartTimeContainer(shared_ptr<StartTimeContainer>(new StartTimeContainer {
				{ IdType(), make_shared<StartTimeImpl>(6, 0) }
			})).
			build()
		);

	irrigationDocument->getPrograms().insert(
		IdType(),
		Program::Builder().
			setName("kanikula").
			setEnabled(true).
			setAdjustment(100).
			setSchedulerType(SchedulerType::HOT_WEATHER).
			setHotWeatherScheduler(make_shared<HotWeatherSchedulerImpl>(
				TemperatureHandler::getInstance().getTemperatureHistory(),
				chrono::hours(2), 33
			)).
			setRunTimeContainer(shared_ptr<RunTimeContainer>(new RunTimeContainer {
				std::make_shared<RunTimeImpl>(chrono::minutes(2) + chrono::seconds(30)),
				std::make_shared<RunTimeImpl>(chrono::minutes(3) + chrono::seconds(30)),
				std::make_shared<RunTimeImpl>(chrono::minutes(3)),
				std::make_shared<RunTimeImpl>(chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(chrono::minutes(0))
			})).
			setStartTimeContainer(shared_ptr<StartTimeContainer>(new StartTimeContainer {
				{ IdType(), make_shared<StartTimeImpl>(12, 0) },
				{ IdType(), make_shared<StartTimeImpl>(13, 0) },
				{ IdType(), make_shared<StartTimeImpl>(14, 0) },
				{ IdType(), make_shared<StartTimeImpl>(15, 0) },
				{ IdType(), make_shared<StartTimeImpl>(16, 0) },
				{ IdType(), make_shared<StartTimeImpl>(17, 0) }
			})).
			build()
		);

	irrigationDocument->getPrograms().insert(
		IdType(),
		Program::Builder().
			setName("paradicsom").
			setEnabled(true).
			setAdjustment(100).
			setSchedulerType(SchedulerType::TEMPERATURE_DEPENDENT).
			setTemperatureDependentScheduler(make_shared<TemperatureDependentSchedulerImpl>(
				TemperatureHandler::getInstance().getTemperatureForecast(),
				TemperatureHandler::getInstance().getTemperatureHistory(),
				1.0f,
				75, 75,
				0
			)).
			setRunTimeContainer(shared_ptr<RunTimeContainer>(new RunTimeContainer {
				std::make_shared<RunTimeImpl>(chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(chrono::minutes(1)),
				std::make_shared<RunTimeImpl>(chrono::minutes(0))
			})).
			setStartTimeContainer(shared_ptr<StartTimeContainer>(new StartTimeContainer {
				{ IdType(), make_shared<StartTimeImpl>(7, 0) },
				{ IdType(), make_shared<StartTimeImpl>(20, 0) }
			})).
			build()
		);

	irrigationDocument->getPrograms().insert(
		IdType(),
		Program::Builder().
			setName("virag").
			setEnabled(true).
			setAdjustment(100).
			setSchedulerType(SchedulerType::TEMPERATURE_DEPENDENT).
			setTemperatureDependentScheduler(make_shared<TemperatureDependentSchedulerImpl>(
				TemperatureHandler::getInstance().getTemperatureForecast(),
				TemperatureHandler::getInstance().getTemperatureHistory(),
				1.0f,
				75, 75,
				0
			)).
			setRunTimeContainer(shared_ptr<RunTimeContainer>(new RunTimeContainer {
				std::make_shared<RunTimeImpl>(chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(chrono::minutes(0)),
				std::make_shared<RunTimeImpl>(chrono::minutes(2))
			})).
			setStartTimeContainer(shared_ptr<StartTimeContainer>(new StartTimeContainer {
				{ IdType(), make_shared<StartTimeImpl>(7, 10) },
				{ IdType(), make_shared<StartTimeImpl>(20, 10) }
			})).
			build()
		);

	irrigationDocument->setModified();
}
