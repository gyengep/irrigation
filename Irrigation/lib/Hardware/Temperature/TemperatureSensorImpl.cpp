#include "TemperatureSensorImpl.h"
#include "TemperatureSensorReader.h"
#include "TemperatureException.h"
#include "Logger/Logger.h"

using namespace std;


TemperatureSensorImpl::TemperatureSensorImpl(const std::shared_ptr<TemperatureSensorReader>& sensorReader) :
	sensorReader(sensorReader),
	valid(false),
	value(0.0f)
{
}

TemperatureSensorImpl::~TemperatureSensorImpl() {
}

float TemperatureSensorImpl::getCachedValue() const {
	lock_guard<mutex> lock(mtx);

	if (!valid) {
		throw TemperatureException("Temperature is not available");
	}

	return value;
}

void TemperatureSensorImpl::updateCache() {
	float value = 0.0f;
	bool valid = false;

	try {
		value = sensorReader->read();
		valid = true;
	} catch(const exception& e) {
		LOGGER.warning("Can not read temperature sensorReader", e);
	}

	lock_guard<mutex> lock(mtx);
	this->value = value;
	this->valid = valid;
}

void TemperatureSensorImpl::startTimer(const std::chrono::seconds& period) {
	timer.reset(new Timer(this, period, Timer::ScheduleType::FIXED_DELAY));
	timer->start();
}

void TemperatureSensorImpl::stopTimer() {
	timer->stop();
	timer.reset();
}

void TemperatureSensorImpl::onTimer() {
#ifdef ONTIMER_TRACE_LOG
	LOGGER.trace("TemperatureSensorReader::onTimer()");
#endif

	updateCache();
}

void TemperatureSensorImpl::addListener(TimerCallback* timerCallback) {
	timer->add(timerCallback);
}

void TemperatureSensorImpl::removeListener(TimerCallback* timerCallback) {
	timer->remove(timerCallback);
}
