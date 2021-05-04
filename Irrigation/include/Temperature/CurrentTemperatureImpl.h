#pragma once
#include <list>
#include <memory>
#include <mutex>
#include "Utils/Runnable.h"
#include "CurrentTemperature.h"

class CurrentTemperatureProvider;


class CurrentTemperatureImpl : public CurrentTemperature, public Runnable {
	const std::shared_ptr<CurrentTemperatureProvider> provider;

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

	virtual void run() override { return updateCache(); }

	virtual float getCurrentTemperature() const override;
	virtual void addListener(CurrentTemperatureListener* currentTemperatureListener) override;
	virtual void removeListener(CurrentTemperatureListener* currentTemperatureListener) override;
};
