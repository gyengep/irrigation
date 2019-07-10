#pragma once
#include "Utils/Timer.h"


class TemperatureSensor {
public:
	virtual ~TemperatureSensor() = default;
	virtual float getCachedValue() const = 0;
	virtual void addListener(TimerCallback* timerCallback) = 0;
	virtual void removeListener(TimerCallback* timerCallback) = 0;
};
