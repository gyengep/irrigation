#include "Temperature.h"
#include "TemperatureForecast.h"
#include "TemperatureHistory.h"
#include "TemperatureSensorDS18B20.h"
#include "TemperatureSensorFake.h"
#include "TemperatureStatisticsImpl.h"
#include "Logger/Logger.h"
#include "Utils/CsvReaderImpl.h"
#include "Utils/CsvWriterImpl.h"
#include <chrono>

using namespace std;

///////////////////////////////////////////////////////////////////////////////

shared_ptr<Temperature> Temperature::instance;
const chrono::seconds::rep Temperature::periodInSeconds(chrono::duration_cast<chrono::seconds>(chrono::hours(24)).count());


void Temperature::init(
		const chrono::duration<int64_t>& sensorUpdatePeriod,
		const string& temperatureCacheFileName,
		const chrono::duration<int64_t>& temperatureCacheLength,
		const string& temperatureHistoryFileName,
		const chrono::duration<int64_t>& temperatureHistoryPeriod,
		const chrono::duration<int64_t>& forecastUpdatePeriod
	)
{
	instance = shared_ptr<Temperature>(new Temperature(
			sensorUpdatePeriod,
			temperatureCacheFileName,
			temperatureCacheLength,
			temperatureHistoryFileName,
			temperatureHistoryPeriod,
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
		const string& temperatureCacheFileName,
		const chrono::duration<int64_t>& temperatureCacheLength,
		const string& temperatureHistoryFileName,
		const chrono::duration<int64_t>& temperatureHistoryPeriod,
		const chrono::duration<int64_t>& forecastUpdatePeriod
	) :
	lastUpdate(time(nullptr)),
	periodStart(0),
	periodEnd(0)
{
	sensor = createSensor();
	sensor->onTimer();

	statistics = make_shared<TemperatureStatisticsImpl>(
			temperatureCacheLength,
			temperatureCacheFileName,
			make_shared<CsvReaderImplFactory>(),
			make_shared<CsvWriterImplFactory>(),
			sensor
		);
	statistics->onTimer();

	history = make_shared<TemperatureHistory>(
			statistics,
			temperatureHistoryPeriod,
			temperatureHistoryFileName,
			make_shared<CsvWriterImplFactory>()
		);
	history->onTimer();
	history->startTimer();

	forecast = make_shared<TemperatureForecast>();
	forecast->onTimer();
	forecast->startTimer(forecastUpdatePeriod);

	sensorTimer.reset(new Timer(sensorUpdatePeriod, Timer::ScheduleType::FIXED_DELAY));
	sensorTimer->add(sensor.get());
	sensorTimer->add(statistics.get());
	sensorTimer->start();

	timer.reset(new Timer(this, chrono::minutes(1), Timer::ScheduleType::FIXED_DELAY));
	timer->start();
}

Temperature::~Temperature() {
	timer->stop();
	timer.reset();

	sensorTimer->stop();
	sensorTimer.reset();

	history->stopTimer();
	forecast->stopTimer();
}

void Temperature::onTimer() {
	const auto currentTime = time(nullptr);

	if ((lastUpdate / periodInSeconds) != (currentTime / periodInSeconds)) {
		lastUpdate = currentTime;

		LOGGER.trace("Temperature::onTimer()");

		logStoredForecast();
		logPreviousPeriodMeasured(currentTime);

		try {
			const auto currentDayStartEnd = getCurrentPeriod(currentTime);
			periodStart = currentDayStartEnd.first;
			periodEnd = currentDayStartEnd.second;
			forecastValues.reset(new TemperatureForecast::Values(forecast->getForecastValues(periodStart, periodEnd)));
		} catch (const TemperatureException& e) {
			LOGGER.trace("Temperature forecast for next day: not available", e);
		}

		logStoredForecast();
	} else {
		//LOGGER.trace("Temperature::onTimer() SKIPPED");
	}
}

void Temperature::logStoredForecast() {
	if (nullptr != forecastValues.get() && 0 != periodStart && 0 != periodEnd) {

		const string start = toTimeStr(periodStart);
		const string end = toTimeStr(periodEnd);

		LOGGER.trace("Temperature forecast\n\tfrom: %s\n\tto:   %s\n\tmin: %.1f, max: %.1f",
				start.c_str(),
				end.c_str(),
				forecastValues->min,
				forecastValues->max
			);
	}
}

void Temperature::logPreviousPeriodMeasured(time_t currentTime) {
	try {
		const auto previousDayStartEnd = getPreviousPeriod(currentTime);

		const string start = toTimeStr(previousDayStartEnd.first);
		const string end = toTimeStr(previousDayStartEnd.second);
		const auto temperatureValues = statistics->getStatisticsValues(periodStart, periodEnd);

		LOGGER.trace("Measured temperature\n\tfrom: %s\n\tto:   %s\n\tmin: %.1f, max: %.1f, avg: %.1f",
				start.c_str(),
				end.c_str(),
				temperatureValues.min,
				temperatureValues.max,
				temperatureValues.avg
			);

	} catch (const TemperatureException& e) {
		LOGGER.trace("Measured temperature\n\tCan not read temperature", e);
	}
}

pair<time_t, time_t> Temperature::getPreviousPeriod(time_t currentTime) {
	return make_pair<time_t, time_t>(
			((currentTime / periodInSeconds) - 1 ) * periodInSeconds,
			(currentTime / periodInSeconds ) * periodInSeconds
		);
}

pair<time_t, time_t> Temperature::getCurrentPeriod(time_t currentTime) {
	return make_pair<time_t, time_t>(
			currentTime / periodInSeconds * periodInSeconds,
			((currentTime / periodInSeconds ) + 1 ) * periodInSeconds
		);
}

shared_ptr<TemperatureSensor> Temperature::createSensor() {
	try {
		return make_shared<TemperatureSensor_DS18B20>();
	} catch (const exception& e) {
		LOGGER.warning("Can not initialize DS18B20 temperature sensor", e);
		return make_shared<TemperatureSensorFake>();
	}
}

string Temperature::toTimeStr(time_t rawTime) {
	struct tm * timeinfo;
	char buffer [80];

	timeinfo = localtime(&rawTime);

	strftime(buffer, 80, "%F %T",timeinfo);
	return buffer;
}
