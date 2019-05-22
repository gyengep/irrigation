#include "Temperature.h"
#include "TemperatureSensorDS18B20.h"
#include "TemperatureSensorFake.h"
#include "TemperaturePersister.h"
#include "TemperatureStatistics.h"
#include "Logger/Logger.h"
#include "Utils/CsvWriterImpl.h"

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

Temperature::Temperature(const string& temperatureFileName) {
	if ((sensor = TemperatureSensor_DS18B20::create()) == nullptr) {
		sensor = make_shared<TemperatureSensor_Fake>();
	}

	auto temperaturePersister = TemperaturePersister::create(temperatureFileName);
	statistics = make_shared<TemperatureStatistics>(sensor, temperaturePersister);

	sensor->startTimer();
	statistics->startTimer();
}

Temperature::~Temperature() {
	sensor->stopTimer();
	statistics->stopTimer();
}

float Temperature::getTemperature() {
	return sensor->getCachedValue();
}
