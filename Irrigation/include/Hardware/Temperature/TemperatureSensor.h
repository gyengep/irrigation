#pragma once
#include <memory>
#include "Utils/Timer.h"

class TemperatureSensorReader;


class TemperatureSensor : public TimerCallback {
	const std::shared_ptr<TemperatureSensorReader> sensorReader;

	std::unique_ptr<Timer> timer;
	mutable std::mutex mtx;
	bool valid;
	float value;

public:
	TemperatureSensor(const std::shared_ptr<TemperatureSensorReader>& sensorReader);
	virtual ~TemperatureSensor();

	float getCachedValue() const;
	void updateCache();

	void startTimer(const std::chrono::duration<int64_t>& period);
	void stopTimer();
	virtual void onTimer() override;
};
