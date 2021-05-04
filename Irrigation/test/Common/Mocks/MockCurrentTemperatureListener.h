#pragma once
#include <gmock/gmock.h>
#include "Temperature/CurrentTemperature.h"


class MockCurrentTemperatureListener : public CurrentTemperatureListener {
public:
	MOCK_METHOD2(onTemperatureUpdated, void(const time_t&, float));
};
