#include "Temperature.h"
#include "TemperatureSensorImpl.h"
#include "TemperatureSensorReaderDS18B20.h"
#include "TemperatureSensorReaderDarkSky.h"
#include "TemperatureSensorReaderOWM.h"
#include "TemperatureHistoryImpl.h"
#include "TemperatureHistoryPersister.h"
#include "TemperatureForecastImpl.h"
#include "TemperatureForecastProviderDarkSky.h"
#include "TemperatureForecastProviderOWM.h"
#include "Logger/Logger.h"
#include "Utils/CsvReaderImpl.h"
#include "Utils/CsvWriterImpl.h"
#include "Utils/TimeConversion.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

Temperature& Temperature::getInstance() {
	static Temperature instance;
	return instance;
}

///////////////////////////////////////////////////////////////////////////////

Temperature::Temperature() :
	lastUpdateTime(time(nullptr)),
	period(chrono::hours(24))
{
}

Temperature::~Temperature() {
}

void Temperature::init(
		const chrono::duration<int64_t>& sensorUpdatePeriod,
		const string& temperatureHistoryFileName,
		const chrono::duration<int64_t>& temperatureHistoryLength,
		const string& temperatureHistoryPersisterFileName,
		const chrono::duration<int64_t>& temperatureHistoryPersisterPeriod,
		const chrono::duration<int64_t>& forecastUpdatePeriod
) {
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

	forecast = make_shared<TemperatureForecastImpl>(
			createForecastProvider()
		);

	sensor->updateCache();
	history->updateCache();
	forecast->updateCache();

	sensor->startTimer(sensorUpdatePeriod);
	history->startTimer();
	historyPersister->startTimer(temperatureHistoryPersisterPeriod);
	forecast->startTimer(forecastUpdatePeriod);

	timer.reset(new Timer(chrono::minutes(1), Timer::ScheduleType::FIXED_DELAY, "Temperature"));
	timer->add(this);
	timer->start();
}

void Temperature::uninit() {
	timer->stop();
	timer.reset();

	forecast->stopTimer();
	historyPersister->stopTimer();
	history->stopTimer();
	sensor->stopTimer();
}

const shared_ptr<TemperatureHistory> Temperature::getTemperatureHistory() const {
	return history;
}

const shared_ptr<TemperatureForecast> Temperature::getTemperatureForecast() const {
	return forecast;
}

void Temperature::onTimer() {
	const auto currentTime = time(nullptr);
	const auto periodInSeconds = chrono::duration_cast<chrono::seconds>(period).count();

	if ((lastUpdateTime / periodInSeconds) != (currentTime / periodInSeconds)) {
		lastUpdateTime = currentTime;
		#ifdef ONTIMER_TRACE_LOG
		LOGGER.trace("Temperature::onTimer()");
		#endif

		if (LOGGER.isLoggable(LogLevel::TRACE)) {
			logPreviousPeriodMeasured(currentTime);
			logStoredPeriodForecast();
			logCurrentPeriodForecast(currentTime);
		}

	} else {
		#ifdef ONTIMER_TRACE_LOG
		LOGGER.trace("Temperature::onTimer() SKIPPED");
		#endif
	}
}

void Temperature::logCurrentPeriodForecast(const time_t& rawTime) {
	try {
		const auto currentPeriodFromTo = getCurrentPeriod(rawTime, period);
		const auto forecastValues = forecast->getForecastValues(currentPeriodFromTo.first, currentPeriodFromTo.second);

		storedForecastFrom = toLocalTimeStr(currentPeriodFromTo.first, "%F %T");
		storedForecastTo = toLocalTimeStr(currentPeriodFromTo.second, "%F %T");
		storedForecastValues = unique_ptr<TemperatureForecast::Values>(new TemperatureForecast::Values(forecastValues));

		logStoredPeriodForecast();
	} catch (const exception& e) {
		LOGGER.trace("Temperature forecast\n\tCan not read temperature forecast", e);
	}
}

void Temperature::logStoredPeriodForecast() {

	if (!storedForecastFrom.empty() && !storedForecastTo.empty() && nullptr != storedForecastValues.get()) {
		LOGGER.trace("Temperature forecast\n\tfrom: %s\n\tto:   %s\n\tmin: %.1f, max: %.1f",
				storedForecastFrom.c_str(),
				storedForecastTo.c_str(),
				storedForecastValues->min,
				storedForecastValues->max
			);
	}
}

void Temperature::logPreviousPeriodMeasured(const time_t& rawTime) {
	try {
		const auto previousPeriodFromTo = getPreviousPeriod(rawTime, period);
		const string from = toLocalTimeStr(previousPeriodFromTo.first, "%F %T");
		const string to = toLocalTimeStr(previousPeriodFromTo.second, "%F %T");
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
	try {
		const auto sensor = make_shared<DS18B20::TemperatureSensorReader>();
		LOGGER.debug("DS18B20 temperature sensor is initialized");
		return sensor;
	} catch (const exception& e) {
		LOGGER.warning("Can not initialize DS18B20 temperature sensor", e);
	}

	//return make_shared<OpenWeatherMap::TemperatureSensorReader>();

	const auto sensor = make_shared<DarkSky::TemperatureSensorReader>();
	LOGGER.debug("DarkSky virtual temperature sensor is initialized");
	return sensor;
}

std::shared_ptr<TemperatureForecastProvider> Temperature::createForecastProvider() {
	const auto forecastProvider = make_shared<DarkSky::TemperatureForecastProvider>();
	LOGGER.debug("DarkSky forecast provider is initialized");
	return forecastProvider;
}
