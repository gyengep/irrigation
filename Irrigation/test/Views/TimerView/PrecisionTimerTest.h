#pragma once
#include <gmock/gmock.h>
#include <atomic>
#include <chrono>
#include "Mocks/MockPrecisionTimerCallback.h"



class PrecisionTimerTest : public ::testing::Test {
protected:

	std::chrono::steady_clock::time_point lastCalled;
	time_t lastRawTime;
	std::atomic_bool isTerminateCalled;

	MockPrecisionTimerCallback mockTimerCallback;
	std::unique_ptr<PrecisionTimer> timer;

    virtual void SetUp();
    virtual void TearDown();

public:

	PrecisionTimerTest();

	void checkTimeDiff();
	void checkIfTerminateCalled();
    void waitAndCallStop(PrecisionTimer* timer, MockPrecisionTimerCallback* mockTimerCallback, unsigned waitMs);
};

