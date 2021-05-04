#pragma once
#include <chrono>
#include <list>
#include <memory>
#include <mutex>
#include <vector>
#include "Utils/Thread.h"
#include "CurrentTemperature.h"
#include "CurrentTemperatureProvider.h"


class CurrentTemperatureImpl : public CurrentTemperature {
	const std::shared_ptr<CurrentTemperatureProvider> provider;

	std::unique_ptr<Thread> workerThread;
	std::mutex listenerMutex;
	std::list<CurrentTemperatureListener*> listeners;

	mutable std::mutex mtx;
	float value;
	bool valid;

	void setValue(float value);
	void invalidateValue();

public:
	CurrentTemperatureImpl(const std::shared_ptr<CurrentTemperatureProvider>& provider);
	virtual ~CurrentTemperatureImpl();

	void updateCache();

	void start(const std::chrono::milliseconds& updatePeriod, const std::vector<std::chrono::milliseconds>& delayOnFailed);
	void stop();

	virtual float getCurrentTemperature() const override;
	virtual void addListener(CurrentTemperatureListener* currentTemperatureListener) override;
	virtual void removeListener(CurrentTemperatureListener* currentTemperatureListener) override;
};
