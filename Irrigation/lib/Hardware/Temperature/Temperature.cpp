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

	statistics = make_shared<TemperatureStatisticsImpl>(
			temperatureCacheLength,
			temperatureCacheFileName,
			make_shared<CsvReaderImplFactory>(),
			make_shared<CsvWriterImplFactory>(),
			sensor
		);

	history = make_shared<TemperatureHistory>(
			statistics,
			temperatureHistoryPeriod,
			temperatureHistoryFileName,
			make_shared<CsvWriterImplFactory>()
		);

	forecast = make_shared<TemperatureForecast>();

	sensor->onTimer();
	statistics->onTimer();
	history->onTimer();
	forecast->onTimer();

	timer.scheduleFixedRate(sensor.get(), sensorUpdatePeriod);
	timer.scheduleFixedRate(statistics.get(), sensorUpdatePeriod);
	history->startTimer();
	forecast->startTimer(forecastUpdatePeriod);
	timer.scheduleFixedRate(this, chrono::minutes(1));
}

Temperature::~Temperature() {
}

string toTimeStr(time_t rawTime) {
	struct tm * timeinfo;
	char buffer [80];

	timeinfo = localtime(&rawTime);

	strftime(buffer, 80, "%F %T",timeinfo);
	return buffer;
}

void Temperature::onTimer() {
	const chrono::seconds::rep periodInSeconds = 24 * 60 * 60;
	const auto currentTime = time(nullptr);

	if ((lastUpdate / periodInSeconds) != (currentTime / periodInSeconds)) {
		lastUpdate = currentTime;

		logForecast();
		logMeasured();

		try {
			periodStart = currentTime / periodInSeconds * periodInSeconds;
			periodEnd = ((currentTime / periodInSeconds ) + 1 ) * periodInSeconds;
			forecastValues.reset(new TemperatureForecast::Values(forecast->getForecastValues(periodStart, periodEnd)));
		} catch (const TemperatureException& e) {
			LOGGER.trace("Temperature forecast for next day: not available", e);
		}

		logForecast();
	}
}

void Temperature::logForecast() {
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

void Temperature::logMeasured() {
	if (0 != periodStart && 0 != periodEnd) {
		try {
			const string start = toTimeStr(periodStart);
			const string end = toTimeStr(periodEnd);
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
}

shared_ptr<TemperatureSensor> Temperature::createSensor() {
	try {
		return make_shared<TemperatureSensor_DS18B20>();
	} catch (const exception& e) {
		LOGGER.warning("Can not initialize DS18B20 temperature sensor", e);
		return make_shared<TemperatureSensorFake>();
	}
}
