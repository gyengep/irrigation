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
