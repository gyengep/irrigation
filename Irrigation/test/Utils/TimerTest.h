#pragma once
#include <gmock/gmock.h>
#include <chrono>
#include <vector>
#include "Mocks/MockTimerCallback.h"
#include "Utils/Timer.h"



class TimerTest : public ::testing::Test {
protected:

	std::vector<std::chrono::steady_clock::time_point> callTimes;

    virtual void SetUp();
    virtual void TearDown();

public:

	void saveCallTime();
};
