#pragma once
#include <gmock/gmock.h>
#include "Utils/Timer.h"


class MockTimerCallback : public TimerCallback {
public:
	MOCK_METHOD0(onTimer, void());
};
