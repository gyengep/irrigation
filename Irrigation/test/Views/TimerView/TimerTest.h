#pragma once
#include <gmock/gmock.h>
#include <atomic>
#include <chrono>
#include "Views/TimerView/Timer.h"



class MockTimerCallback : public TimerCallback {
public:
	std::chrono::steady_clock::time_point lastCalled;
	time_t lastRawTime;
	std::atomic_bool isTerminateCalled;

	MockTimerCallback();

	void checkTimeDiff();
	void checkIfTerminateCalled();

	MOCK_METHOD0(onTimer, void());
};

///////////////////////////////////////////////////////////////////////////////

class TimerTest : public ::testing::Test {
protected:

	MockTimerCallback mockTimerCallback;
	std::unique_ptr<Timer> timer;

    virtual void SetUp();
    virtual void TearDown();

public:
	void waitAndCallStop(Timer* timer, MockTimerCallback* mockTimerCallback, unsigned waitMs);
};

