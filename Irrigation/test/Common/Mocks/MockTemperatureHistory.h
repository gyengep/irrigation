#pragma once
#include <gmock/gmock.h>
#include "Temperature/TemperatureHistory.h"


class MockTemperatureHistory : public TemperatureHistory {
public:
	MOCK_CONST_METHOD2(getTemperatureHistory, TemperatureHistory::Values(const DateTime& from, const DateTime& to));
	MOCK_CONST_METHOD3(toTemperatureHistoryDTO, TemperatureHistoryDTO(const DateTime& from, const DateTime& to, const std::string& dateTimeFormat));
};
