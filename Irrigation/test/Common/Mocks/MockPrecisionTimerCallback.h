#pragma once
#include <gmock/gmock.h>
#include "Views/TimerView/PrecisionTimer.h"



class MockPrecisionTimerCallback : public PrecisionTimerCallback {
public:
	MOCK_METHOD0(onTimer, void());
};
