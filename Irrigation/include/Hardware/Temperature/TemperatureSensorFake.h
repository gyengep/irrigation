#pragma once
#include "TemperatureSensor.h"


class TemperatureSensorFake :public TemperatureSensor {
public:
	TemperatureSensorFake();
	virtual ~TemperatureSensorFake();

	virtual float readValueFromSensor() override;
};
