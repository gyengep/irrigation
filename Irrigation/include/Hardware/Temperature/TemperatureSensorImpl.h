#pragma once
#include <memory>
#include "Utils/Timer.h"
#include "TemperatureSensor.h"

class TemperatureSensorReader;


class TemperatureSensorImpl : public TemperatureSensor, public TimerCallback {
	const std::shared_ptr<TemperatureSensorReader> sensorReader;

	std::unique_ptr<Timer> timer;
	mutable std::mutex mtx;
	std::unique_ptr<float> value;

public:
	TemperatureSensorImpl(const std::shared_ptr<TemperatureSensorReader>& sensorReader);
	virtual ~TemperatureSensorImpl();

	virtual float getCachedValue() const override;
	void updateCache();

	void startTimer(const std::chrono::seconds& period);
	void stopTimer();
	virtual void onTimer() override;

	virtual void addListener(TimerCallback* timerCallback) override;
	virtual void removeListener(TimerCallback* timerCallback) override;
};
