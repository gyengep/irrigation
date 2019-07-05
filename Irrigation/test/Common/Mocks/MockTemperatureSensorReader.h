#pragma once
#include <gmock/gmock.h>
#include "Hardware/Temperature/TemperatureSensorReader.h"


class MockTemperatureSensorReader : public TemperatureSensorReader {
public:
	MOCK_METHOD0(read, float());
};
