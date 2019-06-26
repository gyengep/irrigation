#pragma once
#include <mutex>
#include "Utils/Timer.h"


class TemperatureSensor : public TimerCallback {
	mutable std::mutex mtx;
	bool valid;
	float value;

public:
	TemperatureSensor();
	virtual ~TemperatureSensor();

	float getCachedValue() const;
	void updateCache();

	virtual float readValueFromSensor() = 0;
	virtual void onTimer() override;
};
