#pragma once
#include <gmock/gmock.h>
#include "Hardware/Temperature/TemperatureHistory.h"


class MockTemperatureHistory : public TemperatureHistory {
public:
	MOCK_CONST_METHOD2(getHistoryValues, TemperatureHistory::Values(const std::time_t& from, const std::time_t& to));
};
