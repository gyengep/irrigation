#include "TemperatureSensorImpl.h"
#include "TemperatureSensorReader.h"
#include "TemperatureException.h"
#include "Logger/Logger.h"

using namespace std;


TemperatureSensorImpl::TemperatureSensorImpl(const shared_ptr<TemperatureSensorReader>& sensorReader) :
	sensorReader(sensorReader)
{
}

TemperatureSensorImpl::~TemperatureSensorImpl() {
}

float TemperatureSensorImpl::getCachedValue() const {
	lock_guard<mutex> lock(mtx);

	if (value.get() == nullptr) {
		throw TemperatureException("Temperature is not available");
	}

	return *value;
}

void TemperatureSensorImpl::updateCache() {
	try {
		const float newValue = sensorReader->read();

		lock_guard<mutex> lock(mtx);
		value.reset(new float(newValue));

	} catch(const exception& e) {
		LOGGER.warning("Can not read temperature sensorReader", e);

		lock_guard<mutex> lock(mtx);
		value.reset();
	}
}

void TemperatureSensorImpl::startTimer(const chrono::seconds& period) {
	timer.reset(new Timer(period, Timer::ScheduleType::FIXED_DELAY, "TemperatureSensorImpl"));
	timer->add(this);
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
