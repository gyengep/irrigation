#include "TemperatureSensor.h"
#include "TemperatureException.h"

using namespace std;


TemperatureSensor::TemperatureSensor() :
	timer(*this, chrono::seconds(60)),
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

	} catch(const TemperatureException& e) {
		valid = false;
	}

	lock_guard<mutex> lock(mtx);
	this->value = value;
	this->valid = valid;
}

///////////////////////////////////////////////////////////////////////////////

void TemperatureSensor::startTimer() {
	timer.start();
}

void TemperatureSensor::stopTimer() {
	timer.stop();
}

void TemperatureSensor::onTimer() {
	updateCache();
}
