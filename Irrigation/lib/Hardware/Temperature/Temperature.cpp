#include "Temperature.h"
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
	)
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

		timer.reset(new Timer(*this, sensorUpdatePeriod));
		timer->start();
	} catch (const exception& e) {
		LOGGER.warning("Can not initialize DS18B20 temperature sensor", e);
	}
}

Temperature::~Temperature() {
	if (timer != nullptr) {
		timer->stop();
	}
}

float Temperature::getTemperature() {
	return sensor->getCachedValue();
}

void Temperature::onTimer() {
	sensor->updateCache();

	const time_t currentTime = chrono::system_clock::to_time_t(chrono::system_clock::now());

	try {
		const float temperature = sensor->getCachedValue();
		statistics->addTemperature(currentTime, temperature);
		history->periodicUpdate();
	} catch (...) {
		//persister->appendInvalid(currentTime);
	}
}
