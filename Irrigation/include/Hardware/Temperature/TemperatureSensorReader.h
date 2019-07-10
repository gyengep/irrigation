#pragma once


class TemperatureSensorReader {
public:
	virtual ~TemperatureSensorReader() = default;
	virtual float read() = 0;
};
