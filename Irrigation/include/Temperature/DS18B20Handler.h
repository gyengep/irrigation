#pragma once
#include <string>
#include "Hardware/DS18B20/DS18B20TemperatureSensor.h"
#include "CurrentTemperatureProvider.h"


class DS18B20Handler : public CurrentTemperatureProvider {
	DS18B20TemperatureSensor ds18B20TemperatureSensor;

public:
	DS18B20Handler();
	virtual ~DS18B20Handler();

	virtual float readCurrentTemperature() const override;
	virtual std::string getSensorName() const override;
};
