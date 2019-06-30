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
	timer.reset(new Timer(chrono::milliseconds(100), Timer::ScheduleType::FIXED_RATE));
}

void TimerTest::TearDown() {
	timer.reset();
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(TimerTest, onTimerCalled) {
	EXPECT_CALL(mockTimerCallback, onTimer()).Times(1);

	timer->add(&mockTimerCallback);
	timer->start();
	this_thread::sleep_for(chrono::milliseconds(150));
	timer->stop();
}

TEST_F(TimerTest, onTimerOrder) {
	Sequence seq;
	MockTimerCallback mockTimerCallback1;
	MockTimerCallback mockTimerCallback2;
	MockTimerCallback mockTimerCallback3;

	EXPECT_CALL(mockTimerCallback2, onTimer()).Times(1).InSequence(seq);
	EXPECT_CALL(mockTimerCallback1, onTimer()).Times(1).InSequence(seq);
	EXPECT_CALL(mockTimerCallback3, onTimer()).Times(1).InSequence(seq);

	timer->add(&mockTimerCallback2);
	timer->add(&mockTimerCallback1);
	timer->add(&mockTimerCallback3);
	timer->start();
	this_thread::sleep_for(chrono::milliseconds(150));
	timer->stop();
}

TEST_F(TimerTest, onTimerTiming) {
	ON_CALL(mockTimerCallback, onTimer()).WillByDefault(Invoke(this, &TimerTest::checkTimeDiff));
	EXPECT_CALL(mockTimerCallback, onTimer()).Times(3);

	timer->add(&mockTimerCallback);
	timer->start();
	this_thread::sleep_for(chrono::milliseconds(350));
	timer->stop();
}

TEST_F(TimerTest, addDuplicate) {
	timer->add(&mockTimerCallback);
	EXPECT_THROW(timer->add(&mockTimerCallback), logic_error);
}

TEST_F(TimerTest, remove) {
	EXPECT_CALL(mockTimerCallback, onTimer()).Times(1);

	timer->add(&mockTimerCallback);
	timer->start();
	this_thread::sleep_for(chrono::milliseconds(150));
	timer->remove(&mockTimerCallback);
	this_thread::sleep_for(chrono::milliseconds(150));
	timer->stop();
}

TEST_F(TimerTest, removeInvalid) {
	EXPECT_THROW(timer->remove(&mockTimerCallback), logic_error);
}

TEST_F(TimerTest, removeAll) {
	EXPECT_CALL(mockTimerCallback, onTimer()).Times(1);

	timer->add(&mockTimerCallback);
	timer->start();
	this_thread::sleep_for(chrono::milliseconds(150));
	timer->removeAll();
	this_thread::sleep_for(chrono::milliseconds(150));
	timer->stop();
}
