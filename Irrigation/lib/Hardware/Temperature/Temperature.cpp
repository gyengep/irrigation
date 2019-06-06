#include "Temperature.h"
#include "TemperatureSensorDS18B20.h"
#include "TemperatureSensorFake.h"
#include "TemperaturePersister.h"
#include "TemperatureStatistics.h"
#include "Logger/Logger.h"
#include "Utils/CsvReaderImpl.h"
#include "Utils/CsvWriterImpl.h"
#include <chrono>

using namespace std;

///////////////////////////////////////////////////////////////////////////////

shared_ptr<Temperature> Temperature::instance;

void Temperature::init(const string& temperatureFileName) {
	instance = make_shared<Temperature>(temperatureFileName);
}

shared_ptr<Temperature> Temperature::getInstancePtr() {
	return instance;
}

///////////////////////////////////////////////////////////////////////////////

Temperature::Temperature(const string& temperatureFileName) :
	timer(*this, chrono::seconds(60))
{
	if ((sensor = TemperatureSensor_DS18B20::create()) == nullptr) {
		sensor = make_shared<TemperatureSensor_Fake>();
	}

	statistics = make_shared<TemperatureStatistics>(
			chrono::hours(24),
			temperatureFileName,
			make_shared<CsvReaderImplFactory>(),
			make_shared<CsvWriterImplFactory>()
		);
	//persister;
	//temperaturePersister = TemperaturePersister::create(temperatureFileName);

	timer.start();
}

Temperature::~Temperature() {
	timer.stop();
}

float Temperature::getTemperature() {
	return sensor->getCachedValue();
}

StatisticsValues Temperature::getStatistics(const std::chrono::duration<int64_t>& period) {
	time_t currentTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
	return statistics->getStatistics(currentTime - chrono::duration_cast<chrono::seconds>(period).count(), currentTime);
}

void Temperature::onTimer() {
	sensor->updateCache();

	const time_t currentTime = chrono::system_clock::to_time_t(chrono::system_clock::now());

	try {
		const float temperature = sensor->getCachedValue();
		statistics->addTemperature(currentTime, temperature);
		//persister->append(currentTime, temperature);
	} catch (...) {
		//persister->appendInvalid(currentTime);
	}
}
