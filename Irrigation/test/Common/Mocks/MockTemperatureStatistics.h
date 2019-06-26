#pragma once
#include <gmock/gmock.h>
#include "Hardware/Temperature/TemperatureStatistics.h"


class MockTemperatureStatistics : public TemperatureStatistics {
public:
	MOCK_METHOD2(addTemperature, void(std::time_t rawTime, float temperature));
	MOCK_CONST_METHOD2(getStatisticsValues, TemperatureStatistics::Values(std::time_t from, std::time_t to));
};
