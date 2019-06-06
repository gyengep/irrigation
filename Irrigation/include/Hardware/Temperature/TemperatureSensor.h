#pragma once
#include <mutex>


class TemperatureSensor {
	mutable std::mutex mtx;
	bool valid;
	float value;

public:
	TemperatureSensor();
	virtual ~TemperatureSensor();

	float getCachedValue() const;
	void updateCache();

	virtual float readValueFromSensor() = 0;
};
