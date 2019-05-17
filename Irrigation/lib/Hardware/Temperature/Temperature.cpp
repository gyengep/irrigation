#include "Temperature.h"
#include "TemperatureSensor.h"
#include "TemperatureSensorDS18B20.h"
#include "TemperatureSensorFake.h"
#include "Logger/Logger.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

shared_ptr<Temperature> Temperature::instance;

void Temperature::init() {
	shared_ptr<TemperatureSensor> sensor;

	if ((sensor = TemperatureSensor_DS18B20::create()) == nullptr) {
		sensor = make_shared<TemperatureSensor_Fake>();
	}

	instance = make_shared<Temperature>(sensor);
	instance->refresh();
	instance->startPeriodicRefresh();
}

shared_ptr<Temperature> Temperature::getInstancePtr() {
	return instance;
}

///////////////////////////////////////////////////////////////////////////////

Temperature::Temperature(const shared_ptr<TemperatureSensor>& sensor) :
	sensor(sensor),
	timer(*this, chrono::seconds(10)),
	valid(false),
	value(0.0f)
{
}

Temperature::~Temperature() {
}

void Temperature::lock() {
	mtx.lock();
}

void Temperature::unlock() {
	mtx.unlock();
}

void Temperature::refresh() {
	try {
		value = sensor->readValueFromSensor();
		valid = true;
	} catch(const exception& e) {
		valid = false;
	}
}

bool Temperature::isValid() const {
	return valid;
}

float Temperature::getCachedValue() const {
	if (!valid) {
		throw logic_error("Temperature::getCachedValue() !valid");
	}

	return value;
}

void Temperature::startPeriodicRefresh() {
	timer.start();
}

void Temperature::stopPeriodicRefresh() {
	timer.stop();
}

void Temperature::onTimer() {
	lock_guard<mutex> lock(mtx);
	refresh();
}
