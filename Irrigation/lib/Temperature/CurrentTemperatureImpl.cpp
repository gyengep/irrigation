#include "CurrentTemperatureImpl.h"
#include "CurrentTemperatureProvider.h"
#include "TemperatureException.h"
#include "Logger/Logger.h"

using namespace std;


CurrentTemperatureImpl::CurrentTemperatureImpl(const shared_ptr<CurrentTemperatureProvider>& provider) :
	provider(provider)
{
	LOGGER.debug("%s temperature sensor is initialized", provider->getSensorName().c_str());
}

CurrentTemperatureImpl::~CurrentTemperatureImpl() {
}

float CurrentTemperatureImpl::getCurrentTemperature() const {
	lock_guard<mutex> lock(mtx);

	if (value.get() == nullptr) {
		throw TemperatureException("Temperature is not available");
	}

	return *value;
}

void CurrentTemperatureImpl::updateCache() {
	try {
		const float newValue = provider->readCurrentTemperature();

		lock_guard<mutex> lock(mtx);
		value.reset(new float(newValue));

	} catch(const exception& e) {
		LOGGER.warning("Can not read temperature sensor", e);

		lock_guard<mutex> lock(mtx);
		value.reset();
	}
}

void CurrentTemperatureImpl::startTimer(const chrono::seconds& period) {
	timer.reset(new Timer(period, Timer::ScheduleType::FIXED_DELAY, "CurrentTemperatureImpl"));
	timer->add(this);
	timer->start();
}

void CurrentTemperatureImpl::stopTimer() {
	timer->stop();
	timer.reset();
}

void CurrentTemperatureImpl::onTimer() {
#ifdef ONTIMER_TRACE_LOG
	LOGGER.trace("CurrentTemperatureProvider::onTimer()");
#endif

	updateCache();
}

void CurrentTemperatureImpl::addListener(TimerCallback* timerCallback) {
	timer->add(timerCallback);
}

void CurrentTemperatureImpl::removeListener(TimerCallback* timerCallback) {
	timer->remove(timerCallback);
}
