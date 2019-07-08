#include "Temperature.h"
#include "TemperatureSensorImpl.h"
#include "TemperatureSensorReaderDS18B20.h"
#include "TemperatureSensorReaderOWM.h"
#include "TemperatureSensorReaderFake.h"
#include "TemperatureHistoryImpl.h"
#include "TemperatureHistoryPersister.h"
#include "TemperatureForecast.h"
#include "TemperatureForecastProviderOWM.h"
#include "Logger/Logger.h"
#include "Utils/CsvReaderImpl.h"
#include "Utils/CsvWriterImpl.h"
#include "Utils/TimePeriod.h"
#include <chrono>

using namespace std;

///////////////////////////////////////////////////////////////////////////////

shared_ptr<Temperature> Temperature::instance;
const chrono::seconds::rep Temperature::periodInSeconds(chrono::duration_cast<chrono::seconds>(chrono::hours(24)).count());


void Temperature::init(
		const chrono::duration<int64_t>& sensorUpdatePeriod,
		const string& temperatureHistoryFileName,
		const chrono::duration<int64_t>& temperatureHistoryLength,
		const string& temperatureHistoryPersisterFileName,
		const chrono::duration<int64_t>& temperatureHistoryPersisterPeriod,
		const chrono::duration<int64_t>& forecastUpdatePeriod
	)
{
	instance = shared_ptr<Temperature>(new Temperature(
			sensorUpdatePeriod,
			temperatureHistoryFileName,
			temperatureHistoryLength,
			temperatureHistoryPersisterFileName,
			temperatureHistoryPersisterPeriod,
			forecastUpdatePeriod
		));
}

void Temperature::uninit() {
	instance.reset();
}

shared_ptr<Temperature> Temperature::getInstancePtr() {
	return instance;
}

///////////////////////////////////////////////////////////////////////////////

Temperature::Temperature(
		const chrono::duration<int64_t>& sensorUpdatePeriod,
		const string& temperatureHistoryFileName,
		const chrono::duration<int64_t>& temperatureHistoryLength,
		const string& temperatureHistoryPersisterFileName,
		const chrono::duration<int64_t>& temperatureHistoryPersisterPeriod,
		const chrono::duration<int64_t>& forecastUpdatePeriod
	) :

	lastUpdateTime(chrono::system_clock::now()),
	period(chrono::hours(1))
{
	sensor = make_shared<TemperatureSensorImpl>(
			createSensorReader()
		);

	history = make_shared<TemperatureHistoryImpl>(
			sensor,
			temperatureHistoryLength,
			temperatureHistoryFileName,
			make_shared<CsvReaderImplFactory>(),
			make_shared<CsvWriterImplFactory>()
		);

	historyPersister = make_shared<TemperatureHistoryPersister>(
			history,
			make_shared<CsvWriterImplFactory>(),
			temperatureHistoryPersisterFileName
		);

	forecast = make_shared<TemperatureForecast>(
			make_shared<TemperatureForecastProviderOWM>()
		);

	sensor->updateCache();
	history->updateCache();
	forecast->updateCache();

	sensor->startTimer(sensorUpdatePeriod);
	history->startTimer();
	historyPersister->startTimer(temperatureHistoryPersisterPeriod);
	forecast->startTimer(forecastUpdatePeriod);

	timer.reset(new Timer(this, chrono::minutes(1), Timer::ScheduleType::FIXED_DELAY));
	timer->start();
}

Temperature::~Temperature() {
	timer->stop();
	timer.reset();

	forecast->stopTimer();
	historyPersister->stopTimer();
	history->stopTimer();
	sensor->stopTimer();
}

void Temperature::onTimer() {
	const auto currentTime = chrono::system_clock::now();
	const auto currentTimeInSeconds = chrono::system_clock::to_time_t(currentTime);
	const auto lastUpdateTimeInSeconds = chrono::system_clock::to_time_t(lastUpdateTime);
	const auto periodInSeconds = chrono::duration_cast<chrono::seconds>(period).count();

	if ((lastUpdateTimeInSeconds / periodInSeconds) != (currentTimeInSeconds / periodInSeconds)) {
		lastUpdateTime = currentTime;
		LOGGER.trace("Temperature::onTimer()");

		logPreviousPeriodMeasured(currentTime);
		logCurrentPeriodForecast(currentTime);

	} else {
		LOGGER.trace("Temperature::onTimer() SKIPPED");
	}
}

void Temperature::logCurrentPeriodForecast(const chrono::system_clock::time_point& currentTime) {
	try {
		const auto currentPeriodFromTo = getCurrentPeriod(currentTime, period);
		const string from = toTimeStr(currentPeriodFromTo.first);
		const string to = toTimeStr(currentPeriodFromTo.second);
		const auto forecastValues = forecast->getForecastValues(
				chrono::system_clock::to_time_t(currentPeriodFromTo.first),
				chrono::system_clock::to_time_t(currentPeriodFromTo.second)
			);

		LOGGER.trace("Temperature forecast\n\tfrom: %s\n\tto:   %s\n\tmin: %.1f, max: %.1f",
				from.c_str(),
				to.c_str(),
				forecastValues.min,
				forecastValues.max
			);
	} catch (const exception& e) {
		LOGGER.trace("Temperature forecast\n\tCan not read temperature forecast", e);
	}
}

void Temperature::logPreviousPeriodMeasured(const chrono::system_clock::time_point& currentTime) {
	try {
		const auto previousPeriodFromTo = getPreviousPeriod(currentTime, period);
		const string from = toTimeStr(previousPeriodFromTo.first);
		const string to = toTimeStr(previousPeriodFromTo.second);
		const auto temperatureValues = history->getHistoryValues(previousPeriodFromTo.first, previousPeriodFromTo.second);

		LOGGER.trace("Measured temperature\n\tfrom: %s\n\tto:   %s\n\tmin: %.1f, max: %.1f, avg: %.1f",
				from.c_str(),
				to.c_str(),
				temperatureValues.min,
				temperatureValues.max,
				temperatureValues.avg
			);

	} catch (const exception& e) {
		LOGGER.trace("Measured temperature\n\tCan not read temperature", e);
	}
}

shared_ptr<TemperatureSensorReader> Temperature::createSensorReader() {
	shared_ptr<TemperatureSensorReader> sensor;

	try {
		sensor = make_shared<TemperatureSensorReader_DS18B20>();
		LOGGER.debug("DS18B20 temperature sensor is initialized");
	} catch (const exception& e) {
		LOGGER.warning("Can not initialize DS18B20 temperature sensor", e);
	}

	sensor = make_shared<TemperatureSensorReader_OWM>();
	LOGGER.debug("OWM temperature sensor is initialized");
	return sensor;
}

string Temperature::toTimeStr(const chrono::system_clock::time_point& timePoint) {
	const time_t rawTime = chrono::system_clock::to_time_t(timePoint);
	struct tm * timeinfo;
	char buffer [80];

	timeinfo = localtime(&rawTime);

	strftime(buffer, 80, "%F %T",timeinfo);
	return buffer;
}
