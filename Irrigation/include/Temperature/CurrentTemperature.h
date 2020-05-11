#pragma once
#include "Utils/Timer.h"


class CurrentTemperature {
public:
	virtual ~CurrentTemperature() = default;
	virtual float getCurrentTemperature() const = 0;

	virtual void addListener(TimerCallback* timerCallback) = 0;
	virtual void removeListener(TimerCallback* timerCallback) = 0;
};
