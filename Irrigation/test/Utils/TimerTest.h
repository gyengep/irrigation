#pragma once
#include <gmock/gmock.h>
#include <chrono>
#include "Mocks/MockTimerCallback.h"
#include "Utils/Timer.h"



class TimerTest : public ::testing::Test {
protected:

	std::chrono::steady_clock::time_point lastCalled;

	MockTimerCallback mockTimerCallback;
	Timer timer;

    virtual void SetUp();
    virtual void TearDown();

public:

	void checkTimeDiff();
};
