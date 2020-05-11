#pragma once
#include <gmock/gmock.h>
#include "Temperature/CurrentTemperatureProvider.h"


class MockCurrentTemperatureProvider : public CurrentTemperatureProvider {
public:
	MOCK_CONST_METHOD0(readCurrentTemperature, float());
	MOCK_CONST_METHOD0(getSensorName, std::string());
};
