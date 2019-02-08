#pragma once
#include <gmock/gmock.h>
#include <atomic>
#include <chrono>
#include "Mocks/MockTimerCallback.h"



class TimerTest : public ::testing::Test {
protected:

	MockTimerCallback mockTimerCallback;
	std::unique_ptr<Timer> timer;

    virtual void SetUp();
    virtual void TearDown();

public:
	void waitAndCallStop(Timer* timer, MockTimerCallback* mockTimerCallback, unsigned waitMs);
};

