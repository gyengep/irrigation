#include "Temperature.h"
#include "TemperatureForecast.h"
#include "TemperatureHistory.h"
#include "TemperatureSensorDS18B20.h"
#include "TemperatureStatisticsImpl.h"
#include "Logger/Logger.h"
#include "Utils/CsvReaderImpl.h"
#include "Utils/CsvWriterImpl.h"
#include <chrono>

using namespace std;

///////////////////////////////////////////////////////////////////////////////

shared_ptr<Temperature> Temperature::instance;

void Temperature::init(
		const std::chrono::duration<int64_t>& sensorUpdatePeriod,
		const std::string& temperatureCacheFileName,
		const std::chrono::duration<int64_t>& temperatureCacheLength,
		const std::string& temperatureHistoryFileName,
		const std::chrono::duration<int64_t>& temperatureHistoryPeriod
	)
{
	instance = shared_ptr<Temperature>(new Temperature(
			sensorUpdatePeriod,
			temperatureCacheFileName,
			temperatureCacheLength,
			temperatureHistoryFileName,
			temperatureHistoryPeriod
		));
}

shared_ptr<Temperature> Temperature::getInstancePtr() {
	return instance;
}

///////////////////////////////////////////////////////////////////////////////

Temperature::Temperature(
		const std::chrono::duration<int64_t>& sensorUpdatePeriod,
		const std::string& temperatureCacheFileName,
		const std::chrono::duration<int64_t>& temperatureCacheLength,
		const std::string& temperatureHistoryFileName,
		const std::chrono::duration<int64_t>& temperatureHistoryPeriod
	) :
	timer(*this, sensorUpdatePeriod)
{
	try {
		sensor = make_shared<TemperatureSensor_DS18B20>();
		statistics = make_shared<TemperatureStatisticsImpl>(
				temperatureCacheLength,
				temperatureCacheFileName,
				make_shared<CsvReaderImplFactory>(),
				make_shared<CsvWriterImplFactory>()
			);

		history = make_shared<TemperatureHistory>(
				temperatureHistoryPeriod,
				statistics,
				temperatureHistoryFileName,
				make_shared<CsvWriterImplFactory>()
			);

		sensor->updateCache();
	} catch (const exception& e) {
		LOGGER.warning("Can not initialize DS18B20 temperature sensor", e);
	}

	try {
		forecast = make_shared<TemperatureForecast>();
		forecast->onTimer();
		forecast->startTimer();
	} catch (const exception& e) {
		LOGGER.warning("Can not initialize weather forecast module", e);
	}

	timer.start();
}

Temperature::~Temperature() {
	forecast->stopTimer();

	timer.stop();
}

float Temperature::getTemperature() {
	return sensor->getCachedValue();
}

void Temperature::onTimer() {
	sensor->updateCache();

	const time_t currentTime = time(nullptr);

	try {
		const float temperature = sensor->getCachedValue();
		statistics->addTemperature(currentTime, temperature);
		history->periodicUpdate();
	} catch (...) {
		//persister->appendInvalid(currentTime);
	}
}
