#include "TimerTest.h"
#include <chrono>
#include <ctime>
#include <thread>

using namespace std;
using namespace testing;



MockTimerCallback::MockTimerCallback() :
	lastRawTime(0),
	isTerminateCalled(false)
{
}

void MockTimerCallback::checkTimeDiff() {
	const chrono::steady_clock::time_point currentTime = chrono::steady_clock::now();

	if (lastCalled == chrono::steady_clock::time_point()) {
		lastCalled = currentTime;
		return;
	}

	const chrono::milliseconds::rep diffInMs = chrono::duration_cast<chrono::milliseconds>(currentTime - lastCalled).count();
	const chrono::milliseconds::rep maxDiffInMs = 20;
	EXPECT_THAT(diffInMs, AllOf(Ge(1000 - maxDiffInMs), Le(1000 + maxDiffInMs)));

	lastCalled = currentTime;
}

void MockTimerCallback::checkIfTerminateCalled() {
	EXPECT_FALSE(isTerminateCalled);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void TimerTest::SetUp() {
	timer.reset(new Timer(mockTimerCallback, chrono::seconds(1)));
}

void TimerTest::TearDown() {
}

void TimerTest::waitAndCallStop(Timer* timer, MockTimerCallback* mockTimerCallback, unsigned waitMs) {
	this_thread::sleep_for(chrono::milliseconds(waitMs));
	timer->stop();
	mockTimerCallback->isTerminateCalled = true;
}

TEST_F(TimerTest, onTimerTiming) {
	ON_CALL(mockTimerCallback, onTimer()).WillByDefault(Invoke(&mockTimerCallback, &MockTimerCallback::checkTimeDiff));
	EXPECT_CALL(mockTimerCallback, onTimer()).Times(4);

    thread waitAndCallTerminateThread(&TimerTest::waitAndCallStop, this,
    		timer.get(), &mockTimerCallback, 3500);

    timer->start();
	waitAndCallTerminateThread.join();
}

TEST_F(TimerTest, onTimerAfterTerminate) {
	ON_CALL(mockTimerCallback, onTimer()).WillByDefault(Invoke(&mockTimerCallback, &MockTimerCallback::checkIfTerminateCalled));
	EXPECT_CALL(mockTimerCallback, onTimer()).Times(2);

    thread waitAndCallTerminateThread(&TimerTest::waitAndCallStop, this,
    		timer.get(), &mockTimerCallback, 1500);

    timer->start();
	waitAndCallTerminateThread.join();
}
/*
TEST_F(TimerTest, onTimerParameter) {
	auto currentTime = chrono::system_clock::now();
	chrono::seconds currentTimeInSec = chrono::duration_cast<chrono::seconds>(currentTime.time_since_epoch());
	chrono::system_clock::time_point startTime(currentTimeInSec + chrono::milliseconds(500));

	EXPECT_CALL(mockTimerCallback, onTimer(chrono::system_clock::to_time_t(startTime + chrono::seconds(0)))).Times(1);
	EXPECT_CALL(mockTimerCallback, onTimer(chrono::system_clock::to_time_t(startTime + chrono::seconds(1)))).Times(1);
	EXPECT_CALL(mockTimerCallback, onTimer(chrono::system_clock::to_time_t(startTime + chrono::seconds(2)))).Times(1);
	EXPECT_CALL(mockTimerCallback, onTimer(chrono::system_clock::to_time_t(startTime + chrono::seconds(3)))).Times(1);

	this_thread::sleep_until(startTime);
    thread waitAndCallTerminateThread(&TimerTest::waitAndCallStop, this,
    		timer.get(), &mockTimerCallback, 3500);

    timer->start();
	waitAndCallTerminateThread.join();
}
*/
