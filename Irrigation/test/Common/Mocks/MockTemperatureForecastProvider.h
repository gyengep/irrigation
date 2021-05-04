#pragma once
#include <gmock/gmock.h>
#include "Temperature/TemperatureForecastProvider.h"


class MockTemperatureForecastProvider : public TemperatureForecastProvider {
public:
	MOCK_CONST_METHOD0(readTemperatureForecast, std::list<ValuesWithTimes>());
	MOCK_CONST_METHOD0(getForecastProviderName, std::string());
};
