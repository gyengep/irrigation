#pragma once
#include <gmock/gmock.h>
#include "Temperature/TemperatureForecast.h"


class MockTemperatureForecast : public TemperatureForecast {
public:
	MOCK_CONST_METHOD2(getTemperatureForecast, Values(const std::time_t& from, const std::time_t& to));
};
