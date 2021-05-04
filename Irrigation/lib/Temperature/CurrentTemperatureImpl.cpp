#include "CurrentTemperatureImpl.h"
#include "CurrentTemperatureProvider.h"
#include "TemperatureException.h"
#include "Logger/Logger.h"
#include <algorithm>

using namespace std;


CurrentTemperatureImpl::CurrentTemperatureImpl(const std::shared_ptr<CurrentTemperatureProvider>& provider) :
	provider(provider),
	value(0.0f),
	valid(false)
{
	if (nullptr == provider) {
		throw std::invalid_argument("CurrentTemperatureImpl::CurrentTemperatureImpl() nullptr == provider");
	}

	LOGGER.debug("%s temperature sensor is initialized", provider->getSensorName().c_str());
}

CurrentTemperatureImpl::~CurrentTemperatureImpl() {
}

void CurrentTemperatureImpl::updateCache() {
	try {
		setValue(provider->readCurrentTemperature());
	} catch(const std::runtime_error& e) {
		invalidateValue();
		throw;
	}
}

void CurrentTemperatureImpl::setValue(float value) {
	unique_lock<mutex> lock1(mtx, std::defer_lock);
	unique_lock<mutex> lock2(listenerMutex, std::defer_lock);
	std::lock(lock1, lock2);

	this->value = value;
	this->valid = true;

	LOGGER.debug("Current temperature successfully updated: %.1fC", value);

	const time_t currentTime = time(nullptr);
	auto onUpdateSuccses = [this, &currentTime](CurrentTemperatureListener* currentTemperatureListener) {
		currentTemperatureListener->onTemperatureUpdated(currentTime, this->value);
	};
	std::for_each(listeners.begin(), listeners.end(), onUpdateSuccses);
}

void CurrentTemperatureImpl::invalidateValue() {
	unique_lock<mutex> lock1(mtx);
	this->valid = false;
}

float CurrentTemperatureImpl::getCurrentTemperature() const {
	lock_guard<mutex> lock(mtx);

	if (false == valid) {
		throw TemperatureException("Temperature is not available");
	}

	return value;
}

void CurrentTemperatureImpl::addListener(CurrentTemperatureListener* currentTemperatureListener) {
	lock_guard<mutex> lock(listenerMutex);
	listeners.push_back(currentTemperatureListener);
}

void CurrentTemperatureImpl::removeListener(CurrentTemperatureListener* currentTemperatureListener) {
	lock_guard<mutex> lock(listenerMutex);
	listeners.remove(currentTemperatureListener);
}
