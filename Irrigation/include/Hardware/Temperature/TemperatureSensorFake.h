#pragma once
#include "TemperatureSensor.h"


class TemperatureSensor_Fake : public TemperatureSensor {
public:
	TemperatureSensor_Fake();
	virtual ~TemperatureSensor_Fake();

	virtual float readValueFromSensor() override;
};
