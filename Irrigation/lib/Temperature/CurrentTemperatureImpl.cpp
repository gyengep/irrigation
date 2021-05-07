#include "CurrentTemperatureImpl.h"
#include "TemperatureException.h"
#include "Logger/Logger.h"
#include "Utils/FixedDelaySchedulerRunnable.h"
#include "Utils/FunctionRunnable.h"
#include "Utils/RepeatUntilSuccessRunnable.h"
#include "Utils/ToString.h"
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

void CurrentTemperatureImpl::start(const std::chrono::milliseconds& updatePeriod, const std::vector<std::chrono::milliseconds>& delayOnFailed) {
	std::chrono::milliseconds firstDelay;

	try {
		updateCache();
		firstDelay = updatePeriod;
	} catch (const std::exception& e) {
		LOGGER.warning("Current temperature update failed", e);
		firstDelay = std::chrono::milliseconds(100);
	}

	auto func = [this] {
		updateCache();
	};

	auto functionRunnbale = std::make_shared<FunctionRunnable>(func);
	auto repeatUntilSuccessRunnable = std::make_shared<RepeatUntilSuccessRunnable>(
			functionRunnbale,
			delayOnFailed,
			"Current temperature update"
		);

	auto fixedDelaySchedulerRunnable =  std::make_shared<FixedDelaySchedulerRunnable>(
			repeatUntilSuccessRunnable,
			firstDelay,
			updatePeriod,
			"CurrentTemperatureImpl"
		);

	workerThread = std::unique_ptr<Thread>(new Thread(
			fixedDelaySchedulerRunnable,
			"CurrentTemperatureImpl"
		));

	workerThread->start();
}

void CurrentTemperatureImpl::stop() {
	workerThread->stop();
	workerThread.reset();
}

void CurrentTemperatureImpl::setValue(float value) {
	unique_lock<mutex> lock1(mtx, std::defer_lock);
	unique_lock<mutex> lock2(listenerMutex, std::defer_lock);
	std::lock(lock1, lock2);

	this->value = value;
	this->valid = true;

	LOGGER.debug("Current temperature successfully updated: %s", toCelsius(value).c_str());

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
