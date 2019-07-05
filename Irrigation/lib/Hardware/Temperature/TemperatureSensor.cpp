#include "TemperatureSensor.h"
#include "TemperatureSensorReader.h"
#include "TemperatureException.h"
#include "Logger/Logger.h"

using namespace std;


TemperatureSensor::TemperatureSensor(const std::shared_ptr<TemperatureSensorReader>& sensorReader) :
	sensorReader(sensorReader),
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
		value = sensorReader->read();
		valid = true;
	} catch(const TemperatureException& e) {
		LOGGER.warning("Can not read temperature sensorReader", e);
	}

	lock_guard<mutex> lock(mtx);
	this->value = value;
	this->valid = valid;
}

void TemperatureSensor::startTimer(const std::chrono::duration<int64_t>& period) {
	timer.reset(new Timer(this, period, Timer::ScheduleType::FIXED_DELAY));
	timer->start();
}

void TemperatureSensor::stopTimer() {
	timer->stop();
	timer.reset();
}

void TemperatureSensor::onTimer() {
	LOGGER.trace("TemperatureSensorReader::onTimer()");
	updateCache();
}
