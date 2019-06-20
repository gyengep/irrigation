#pragma once
#include <gmock/gmock.h>
#include "Hardware/Temperature/TemperatureStatistics.h"


class MockTemperatureStatistics : public TemperatureStatistics {
public:
	MOCK_METHOD2(addTemperature, void(std::time_t rawTime, float temperature));
	MOCK_METHOD2(getStatistics, TemperatureValues(std::time_t from, std::time_t to));
};
