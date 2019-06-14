#include "TemperatureSensor.h"
#include "TemperatureException.h"
#include "Logger/Logger.h"

using namespace std;


TemperatureSensor::TemperatureSensor() :
	valid(false),
	value(0.0f)
{
}

TemperatureSensor::~TemperatureSensor() {
}

float TemperatureSensor::getCachedValue() const {
	lock_guard<mutex> lock(mtx);

	if (!valid) {
		throw TemperatureException("Temperature is not available");
	}

	return value;
}

void TemperatureSensor::updateCache() {
	float value = 0.0f;
	bool valid = false;

	try {
		value = readValueFromSensor();
		valid = true;

	} catch(const exception& e) {
		LOGGER.warning("Can not read temperature from DS18B20 sensor", e);
	}

	lock_guard<mutex> lock(mtx);
	this->value = value;
	this->valid = valid;
}
