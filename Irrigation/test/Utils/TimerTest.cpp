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

void TimerTest::SetUp() {
}

void TimerTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(TimerTest, onTimerCalled) {
	EXPECT_CALL(mockTimerCallback, onTimer()).Times(1);

	timer.scheduleFixedDelay(&mockTimerCallback, chrono::milliseconds(100));
	this_thread::sleep_for(chrono::milliseconds(150));
}

TEST_F(TimerTest, onTimerOrder) {
	Sequence seq;
	MockTimerCallback mockTimerCallback1;
	MockTimerCallback mockTimerCallback2;
	MockTimerCallback mockTimerCallback3;

	EXPECT_CALL(mockTimerCallback2, onTimer()).Times(1).InSequence(seq);
	EXPECT_CALL(mockTimerCallback1, onTimer()).Times(1).InSequence(seq);
	EXPECT_CALL(mockTimerCallback3, onTimer()).Times(1).InSequence(seq);

	timer.scheduleFixedDelay(&mockTimerCallback2, chrono::milliseconds(100));
	timer.scheduleFixedDelay(&mockTimerCallback1, chrono::milliseconds(100));
	timer.scheduleFixedDelay(&mockTimerCallback3, chrono::milliseconds(100));
	this_thread::sleep_for(chrono::milliseconds(150));
}

TEST_F(TimerTest, onTimerMultipleCallback) {
	Sequence seq;
	MockTimerCallback mockTimerCallback1;
	MockTimerCallback mockTimerCallback2;
	MockTimerCallback mockTimerCallback3;

	EXPECT_CALL(mockTimerCallback1, onTimer()).Times(1).InSequence(seq);
	EXPECT_CALL(mockTimerCallback2, onTimer()).Times(1).InSequence(seq);
	EXPECT_CALL(mockTimerCallback3, onTimer()).Times(1).InSequence(seq);
	EXPECT_CALL(mockTimerCallback1, onTimer()).Times(1).InSequence(seq);
	EXPECT_CALL(mockTimerCallback2, onTimer()).Times(1).InSequence(seq);
	EXPECT_CALL(mockTimerCallback1, onTimer()).Times(1).InSequence(seq);
	EXPECT_CALL(mockTimerCallback3, onTimer()).Times(1).InSequence(seq);
	EXPECT_CALL(mockTimerCallback1, onTimer()).Times(1).InSequence(seq);
	EXPECT_CALL(mockTimerCallback2, onTimer()).Times(1).InSequence(seq);
	EXPECT_CALL(mockTimerCallback1, onTimer()).Times(1).InSequence(seq);

	// 100: m1
	// 150:     m2
	// 200: m1,     m3
	// 300: m1, m2
	// 400: m1,     m3
	// 450:     m2
	// 500: m1

	timer.scheduleFixedDelay(&mockTimerCallback2, chrono::milliseconds(150));
	timer.scheduleFixedDelay(&mockTimerCallback1, chrono::milliseconds(100));
	timer.scheduleFixedDelay(&mockTimerCallback3, chrono::milliseconds(200));
	this_thread::sleep_for(chrono::milliseconds(550));
}

TEST_F(TimerTest, onTimerTiming) {
	ON_CALL(mockTimerCallback, onTimer()).WillByDefault(Invoke(this, &TimerTest::checkTimeDiff));
	EXPECT_CALL(mockTimerCallback, onTimer()).Times(3);

	timer.scheduleFixedDelay(&mockTimerCallback, chrono::milliseconds(100));
	this_thread::sleep_for(chrono::milliseconds(350));
}
TEST_F(TimerTest, addDuplicate) {
	timer.scheduleFixedDelay(&mockTimerCallback, chrono::milliseconds(100));
	EXPECT_THROW(timer.scheduleFixedDelay(&mockTimerCallback, chrono::milliseconds(100)), logic_error);
	EXPECT_THROW(timer.scheduleFixedRate(&mockTimerCallback, chrono::milliseconds(100)), logic_error);
}

TEST_F(TimerTest, remove) {
	EXPECT_CALL(mockTimerCallback, onTimer()).Times(1);

	timer.scheduleFixedDelay(&mockTimerCallback, chrono::milliseconds(100));
	this_thread::sleep_for(chrono::milliseconds(150));
	timer.remove(&mockTimerCallback);
	this_thread::sleep_for(chrono::milliseconds(150));
}

TEST_F(TimerTest, removeInvalid) {
	EXPECT_THROW(timer.remove(&mockTimerCallback), logic_error);
}

TEST_F(TimerTest, cancel) {
	EXPECT_CALL(mockTimerCallback, onTimer()).Times(1);

	timer.scheduleFixedDelay(&mockTimerCallback, chrono::milliseconds(100));
	this_thread::sleep_for(chrono::milliseconds(150));
	timer.cancel();
	this_thread::sleep_for(chrono::milliseconds(150));
}
