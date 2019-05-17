#include <chrono>
#include <thread>
#include "TimerTest.h"

using namespace std;
using namespace testing;


TimerTest::TimerTest() :
	isTerminateCalled(false)
{
}

void TimerTest::checkTimeDiff() {
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

void TimerTest::checkIfTerminateCalled() {
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
	isTerminateCalled = true;
}

TEST_F(TimerTest, onTimerTiming) {
	ON_CALL(mockTimerCallback, onTimer()).WillByDefault(Invoke(this, &TimerTest::checkTimeDiff));
	EXPECT_CALL(mockTimerCallback, onTimer()).Times(3);

    thread waitAndCallTerminateThread(&TimerTest::waitAndCallStop, this,
    		timer.get(), &mockTimerCallback, 3500);

    timer->start();
	waitAndCallTerminateThread.join();
}

TEST_F(TimerTest, onTimerAutoStart) {
	timer.reset(new Timer(mockTimerCallback, chrono::seconds(1), true));

	EXPECT_CALL(mockTimerCallback, onTimer()).Times(1);

    thread waitAndCallTerminateThread(&TimerTest::waitAndCallStop, this,
    		timer.get(), &mockTimerCallback, 1500);

	waitAndCallTerminateThread.join();
}

TEST_F(TimerTest, onTimerNotAutoStart) {
	timer.reset(new Timer(mockTimerCallback, chrono::seconds(1)));

	EXPECT_CALL(mockTimerCallback, onTimer()).Times(0);

    thread waitAndCallTerminateThread(&TimerTest::waitAndCallStop, this,
    		timer.get(), &mockTimerCallback, 1500);

	waitAndCallTerminateThread.join();
}

TEST_F(TimerTest, onTimerAfterTerminate) {
	ON_CALL(mockTimerCallback, onTimer()).WillByDefault(Invoke(this, &TimerTest::checkIfTerminateCalled));
	EXPECT_CALL(mockTimerCallback, onTimer()).Times(1);

    thread waitAndCallTerminateThread(&TimerTest::waitAndCallStop, this,
    		timer.get(), &mockTimerCallback, 1500);

    timer->start();
	waitAndCallTerminateThread.join();
}
