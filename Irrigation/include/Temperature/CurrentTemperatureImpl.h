#pragma once
#include <memory>
#include "Utils/Timer.h"
#include "CurrentTemperature.h"

class CurrentTemperatureProvider;


class CurrentTemperatureImpl : public CurrentTemperature, public TimerCallback {
	const std::shared_ptr<CurrentTemperatureProvider> provider;

	mutable std::mutex mtx;
	std::unique_ptr<Timer> timer;
	std::unique_ptr<float> value;

public:
	CurrentTemperatureImpl(const std::shared_ptr<CurrentTemperatureProvider>& provider);
	virtual ~CurrentTemperatureImpl();

	virtual float getCurrentTemperature() const override;
	void updateCache();

	void startTimer(const std::chrono::seconds& period);
	void stopTimer();
	virtual void onTimer() override;

	virtual void addListener(TimerCallback* timerCallback) override;
	virtual void removeListener(TimerCallback* timerCallback) override;
};
