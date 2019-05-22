#include <chrono>
#include <thread>
#include "TimerTest.h"

using namespace std;
using namespace testing;


void TimerTest::checkTimeDiff() {
	const chrono::steady_clock::time_point currentTime = chrono::steady_clock::now();

	if (lastCalled == chrono::steady_clock::time_point()) {
		lastCalled = currentTime;
		return;
	}

	const chrono::milliseconds::rep diffInMs = chrono::duration_cast<chrono::milliseconds>(currentTime - lastCalled).count();
	const chrono::milliseconds::rep maxDiffInMs = 10;
	EXPECT_THAT(diffInMs, AllOf(Ge(100 - maxDiffInMs), Le(100 + maxDiffInMs)));

	lastCalled = currentTime;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void TimerTest::SetUp() {
	timer.reset(new Timer(mockTimerCallback, chrono::milliseconds(100)));
}

void TimerTest::TearDown() {
}
TEST_F(TimerTest, onTimerCalled) {
	EXPECT_CALL(mockTimerCallback, onTimer()).Times(1);

	timer->start();
	this_thread::sleep_for(chrono::milliseconds(150));
    timer->stop();
}

TEST_F(TimerTest, onTimerAfterTerminate) {
	EXPECT_CALL(mockTimerCallback, onTimer()).Times(1);

	timer->start();
	this_thread::sleep_for(chrono::milliseconds(150));
    timer->stop();
	this_thread::sleep_for(chrono::milliseconds(150));
}


TEST_F(TimerTest, onTimerTiming) {
	ON_CALL(mockTimerCallback, onTimer()).WillByDefault(Invoke(this, &TimerTest::checkTimeDiff));
	EXPECT_CALL(mockTimerCallback, onTimer()).Times(3);

	timer->start();
	this_thread::sleep_for(chrono::milliseconds(350));
    timer->stop();
}

TEST_F(TimerTest, onTimerNotStart) {
	EXPECT_CALL(mockTimerCallback, onTimer()).Times(0);

	this_thread::sleep_for(chrono::milliseconds(150));
}
