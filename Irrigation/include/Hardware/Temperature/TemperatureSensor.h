#pragma once
#include "Utils/Timer.h"


class TemperatureSensor : public TimerCallback {
	mutable std::mutex mtx;
	Timer timer;
	bool valid;
	float value;

public:
	TemperatureSensor();
	virtual ~TemperatureSensor();

	float getCachedValue() const;
	void updateCache();

	virtual float readValueFromSensor() = 0;

	void startTimer();
	void stopTimer();
	virtual void onTimer() override;
};
