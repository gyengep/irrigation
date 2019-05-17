#pragma once
#include <gmock/gmock.h>
#include "Hardware/Temperature/TemperatureSensor.h"


class MockTemperatureSensor : public TemperatureSensor {
public:
	MOCK_METHOD0(readValueFromSensor, float());
};
