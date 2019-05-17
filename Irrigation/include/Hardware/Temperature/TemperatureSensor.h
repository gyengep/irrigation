#pragma once


class TemperatureSensor {
public:
	virtual ~TemperatureSensor() = default;
	virtual float readValueFromSensor() = 0;
};
