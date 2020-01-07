#pragma once
#include <string>


class CurrentTemperatureProvider {
public:
	virtual ~CurrentTemperatureProvider() = default;
	virtual float readCurrentTemperature() const = 0;
	virtual std::string getSensorName() const = 0;
};
