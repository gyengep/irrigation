#pragma once
#include <gmock/gmock.h>
#include "Temperature/TemperatureForecast.h"


class MockTemperatureForecast : public TemperatureForecast {
public:
	MOCK_CONST_METHOD2(getTemperatureForecast, Values(const DateTime& from, const DateTime& to));
	MOCK_CONST_METHOD3(toTemperatureForecastDTO, TemperatureForecastDTO(const DateTime& from, const DateTime& to, const std::string& dateTimeFormat));
};
