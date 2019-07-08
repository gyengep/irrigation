#pragma once
#include <gmock/gmock.h>
#include "Hardware/Temperature/TemperatureSensor.h"


class MockTemperatureSensor : public TemperatureSensor {
public:
	MOCK_METHOD1(addListener, void(TimerCallback* timerCallback));
	MOCK_METHOD1(removeListener, void(TimerCallback* timerCallback));

	MOCK_CONST_METHOD0(getCachedValue, float());
};
