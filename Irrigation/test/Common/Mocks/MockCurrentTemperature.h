#pragma once
#include <gmock/gmock.h>
#include "Temperature/CurrentTemperature.h"


class MockCurrentTemperature : public CurrentTemperature {
public:
	MOCK_METHOD1(addListener, void(TimerCallback* timerCallback));
	MOCK_METHOD1(removeListener, void(TimerCallback* timerCallback));
	MOCK_CONST_METHOD0(getCurrentTemperature, float());
};
