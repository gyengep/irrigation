#pragma once
#include "Utils/DateTime.h"


class CurrentTemperatureListener {
public:
	virtual ~CurrentTemperatureListener() = default;
	virtual void onTemperatureUpdated(const DateTime& dateTime, float temperature) = 0;
};


class CurrentTemperature {
public:
	virtual ~CurrentTemperature() = default;
	virtual float getCurrentTemperature() const = 0;

	virtual void addListener(CurrentTemperatureListener* currentTemperatureListener) = 0;
	virtual void removeListener(CurrentTemperatureListener* currentTemperatureListener) = 0;
};
