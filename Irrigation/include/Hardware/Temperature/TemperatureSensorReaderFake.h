#pragma once
#include "TemperatureSensorReader.h"


class TemperatureSensorReaderFake :public TemperatureSensorReader {
public:
	TemperatureSensorReaderFake();
	virtual ~TemperatureSensorReaderFake();

	virtual float read() override;
};
