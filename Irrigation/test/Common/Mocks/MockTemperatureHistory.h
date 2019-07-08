#pragma once
#include <gmock/gmock.h>
#include "Hardware/Temperature/TemperatureHistory.h"


class MockTemperatureHistory : public TemperatureHistory {
public:
	MOCK_CONST_METHOD2(getHistoryValues, TemperatureHistory::Values(
			const std::chrono::system_clock::time_point& from,
			const std::chrono::system_clock::time_point& to
		));
};
