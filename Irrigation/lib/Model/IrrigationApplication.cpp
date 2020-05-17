#include "IrrigationApplication.h"
#include "Configuration.h"
#include "DocumentSaver.h"
#include "DtoReaderWriter/XMLParseException.h"
#include "DtoReaderWriter/XmlReader.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "Email/Email.h"
#include "Exceptions/Exceptions.h"
#include "Temperature/Temperature.h"
#include "Hardware/Valves/GpioHandler.h"
#include "Logger/Logger.h"
#include "Logic/Program.h"
#include "Logic/ProgramContainer.h"
#include "Logic/RunTime.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTime.h"
#include "Logic/StartTimeContainer.h"
#include "Model/IrrigationDocument.h"
#include "Schedulers/TemperatureDependentScheduler.h"
#include "Schedulers/HotWeatherScheduler.h"
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
	EMAIL.enableTopic(EmailTopic::WATERING_START);
	EMAIL.enableTopic(EmailTopic::WATERING_SKIP);
}

void IrrigationApplication::initGpio() {
	try {
		GpioHandler::init();
	} catch (const exception& e) {
		throw_with_nested(runtime_error("Can't initialize valves"));
	}
}

void IrrigationApplication::initTemperature() {
	try {
		Temperature::getInstance().init(
			Configuration::getInstance().getCurrentTemperatureUpdatePeriod(),
			Configuration::getInstance().getTemperatureCacheFileName(),
			Configuration::getInstance().getTemperatureCacheLength(),
			Configuration::getInstance().getTemperatureHistoryFileName(),
			Configuration::getInstance().getTemperatureHistoryPeriod(),
			Configuration::getInstance().getTemperatureForecastUpdatePeriod()
		);

	} catch (const exception& e) {
		throw_with_nested(runtime_error("Can't initialize temperature module"));
	}
}

void IrrigationApplication::uninitTemperature() {
	try {
		Temperature::getInstance().uninit();
	} catch (const exception& e) {
		LOGGER.warning("An error during uninitialize temperature module", e);
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

	initEmail();
	initGpio();
	initTemperature();
	initDocument();

	LOGGER.info("Irrigation System started");
}

void IrrigationApplication::onTerminate() {
	LOGGER.debug("Irrigation System stopping ... ");

	uninitDocument();
	uninitTemperature();

	LOGGER.info("Irrigation System stopped");
}

void IrrigationApplication::setMyDefaults() {
	irrigationDocument->getPrograms().insert(
		IdType(),
		Program::Builder().
			setName("fulocsolas").
			setDisabled(false).
			setAdjustment(100).
			setSchedulerType(SchedulerType::TEMPERATURE_DEPENDENT).
			setTemperatureDependentScheduler(make_shared<TemperatureDependentScheduler>(
				Temperature::getInstance().getTemperatureForecast(),
				Temperature::getInstance().getTemperatureHistory(),
				0.75f,
				50, 0,
				100
			)).
			setRunTimeContainer(shared_ptr<RunTimeContainer>(new RunTimeContainer {
				RunTime(chrono::minutes(26)),
				RunTime(chrono::minutes(38)),
				RunTime(chrono::minutes(32)),
				RunTime(chrono::minutes(0)),
				RunTime(chrono::minutes(0)),
				RunTime(chrono::minutes(0))
			})).
			setStartTimeContainer(shared_ptr<StartTimeContainer>(new StartTimeContainer {
				{ IdType(), make_shared<StartTime>(4, 0) }
			})).
			build()
		);

	irrigationDocument->getPrograms().insert(
		IdType(),
		Program::Builder().
			setName("buxus").
			setDisabled(false).
			setAdjustment(100).
			setSchedulerType(SchedulerType::TEMPERATURE_DEPENDENT).
			setTemperatureDependentScheduler(make_shared<TemperatureDependentScheduler>(
				Temperature::getInstance().getTemperatureForecast(),
				Temperature::getInstance().getTemperatureHistory(),
				1.0f,
				100, 0,
				0
			)).
			setRunTimeContainer(shared_ptr<RunTimeContainer>(new RunTimeContainer {
				RunTime(chrono::minutes(0)),
				RunTime(chrono::minutes(0)),
				RunTime(chrono::minutes(0)),
				RunTime(chrono::minutes(15)),
				RunTime(chrono::minutes(0)),
				RunTime(chrono::minutes(0))
			})).
			setStartTimeContainer(shared_ptr<StartTimeContainer>(new StartTimeContainer {
				{ IdType(), make_shared<StartTime>(6, 0) }
			})).
			build()
		);

	irrigationDocument->getPrograms().insert(
		IdType(),
		Program::Builder().
			setName("kanikula").
			setDisabled(false).
			setAdjustment(100).
			setSchedulerType(SchedulerType::HOT_WEATHER).
			setHotWeatherScheduler(make_shared<HotWeatherScheduler>(
				Temperature::getInstance().getTemperatureHistory(),
				chrono::hours(2), 33
			)).
			setRunTimeContainer(shared_ptr<RunTimeContainer>(new RunTimeContainer {
				RunTime(chrono::minutes(2) + chrono::seconds(30)),
				RunTime(chrono::minutes(3) + chrono::seconds(30)),
				RunTime(chrono::minutes(3)),
				RunTime(chrono::minutes(0)),
				RunTime(chrono::minutes(0)),
				RunTime(chrono::minutes(0))
			})).
			setStartTimeContainer(shared_ptr<StartTimeContainer>(new StartTimeContainer {
				{ IdType(), make_shared<StartTime>(12, 0) },
				{ IdType(), make_shared<StartTime>(13, 0) },
				{ IdType(), make_shared<StartTime>(14, 0) },
				{ IdType(), make_shared<StartTime>(15, 0) },
				{ IdType(), make_shared<StartTime>(16, 0) },
				{ IdType(), make_shared<StartTime>(17, 0) }
			})).
			build()
		);

	irrigationDocument->getPrograms().insert(
		IdType(),
		Program::Builder().
			setName("paradicsom").
			setDisabled(false).
			setAdjustment(100).
			setSchedulerType(SchedulerType::TEMPERATURE_DEPENDENT).
			setTemperatureDependentScheduler(make_shared<TemperatureDependentScheduler>(
				Temperature::getInstance().getTemperatureForecast(),
				Temperature::getInstance().getTemperatureHistory(),
				1.0f,
				75, 75,
				0
			)).
			setRunTimeContainer(shared_ptr<RunTimeContainer>(new RunTimeContainer {
				RunTime(chrono::minutes(0)),
				RunTime(chrono::minutes(0)),
				RunTime(chrono::minutes(0)),
				RunTime(chrono::minutes(0)),
				RunTime(chrono::minutes(1)),
				RunTime(chrono::minutes(0))
			})).
			setStartTimeContainer(shared_ptr<StartTimeContainer>(new StartTimeContainer {
				{ IdType(), make_shared<StartTime>(7, 0) },
				{ IdType(), make_shared<StartTime>(20, 0) }
			})).
			build()
		);

	irrigationDocument->getPrograms().insert(
		IdType(),
		Program::Builder().
			setName("virag").
			setDisabled(false).
			setAdjustment(100).
			setSchedulerType(SchedulerType::TEMPERATURE_DEPENDENT).
			setTemperatureDependentScheduler(make_shared<TemperatureDependentScheduler>(
				Temperature::getInstance().getTemperatureForecast(),
				Temperature::getInstance().getTemperatureHistory(),
				1.0f,
				75, 75,
				0
			)).
			setRunTimeContainer(shared_ptr<RunTimeContainer>(new RunTimeContainer {
				RunTime(chrono::minutes(0)),
				RunTime(chrono::minutes(0)),
				RunTime(chrono::minutes(0)),
				RunTime(chrono::minutes(0)),
				RunTime(chrono::minutes(0)),
				RunTime(chrono::minutes(2))
			})).
			setStartTimeContainer(shared_ptr<StartTimeContainer>(new StartTimeContainer {
				{ IdType(), make_shared<StartTime>(7, 10) },
				{ IdType(), make_shared<StartTime>(20, 10) }
			})).
			build()
		);

	irrigationDocument->setModified();
}
