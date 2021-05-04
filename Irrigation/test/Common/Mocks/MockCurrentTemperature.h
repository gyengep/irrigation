#pragma once
#include <gmock/gmock.h>
#include "Temperature/CurrentTemperature.h"


class MockCurrentTemperature : public CurrentTemperature {
public:
	MOCK_CONST_METHOD0(getCurrentTemperature, float());
	MOCK_METHOD1(addListener, void(CurrentTemperatureListener* currentTemperatureListener));
	MOCK_METHOD1(removeListener, void(CurrentTemperatureListener* currentTemperatureListener));
};
