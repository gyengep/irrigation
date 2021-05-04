#include <thread>
#include "TestCommon/cout.h"
#include "TimerTest.h"

using namespace std;
using namespace testing;
/*
void TimerTest::saveCallTime() {
	callTimes.push_back(chrono::steady_clock::now());
	this_thread::sleep_for(chrono::milliseconds(50));
}

///////////////////////////////////////////////////////////////////////////////

void TimerTest::SetUp() {
}

void TimerTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(TimerTest, onTimerCalled) {
	MockTimerCallback mockTimerCallback;
	EXPECT_CALL(mockTimerCallback, onTimer()).Times(1);

	Timer timer(chrono::milliseconds(100), Timer::ScheduleType::FIXED_RATE, "");

	timer.add(&mockTimerCallback);
	timer.start();
	this_thread::sleep_for(chrono::milliseconds(150));
	timer.stop();
}

TEST_F(TimerTest, onTimerOrder) {
	Sequence seq;
	MockTimerCallback mockTimerCallback1;
	MockTimerCallback mockTimerCallback2;
	MockTimerCallback mockTimerCallback3;

	EXPECT_CALL(mockTimerCallback2, onTimer()).Times(1).InSequence(seq);
	EXPECT_CALL(mockTimerCallback1, onTimer()).Times(1).InSequence(seq);
	EXPECT_CALL(mockTimerCallback3, onTimer()).Times(1).InSequence(seq);

	Timer timer(chrono::milliseconds(100), Timer::ScheduleType::FIXED_RATE, "");

	timer.add(&mockTimerCallback2);
	timer.add(&mockTimerCallback1);
	timer.add(&mockTimerCallback3);
	timer.start();
	this_thread::sleep_for(chrono::milliseconds(150));
	timer.stop();
}

TEST_F(TimerTest, onTimerFixedRateTiming) {
	const chrono::milliseconds::rep maxDiffInMs = 10;

	MockTimerCallback mockTimerCallback;
	EXPECT_CALL(mockTimerCallback, onTimer()).
			WillRepeatedly(Invoke(this, &TimerTest::saveCallTime));

	Timer timer(chrono::milliseconds(100), Timer::ScheduleType::FIXED_RATE, "");

	timer.add(&mockTimerCallback);
	timer.start();
	this_thread::sleep_for(chrono::milliseconds(350));
	timer.stop();

	ASSERT_THAT(callTimes, SizeIs(3));

	EXPECT_THAT(chrono::duration_cast<chrono::milliseconds>(callTimes[1] - callTimes[0]).count(),
			AllOf(Ge(100 - maxDiffInMs), Le(100 + maxDiffInMs))
		);

	EXPECT_THAT(chrono::duration_cast<chrono::milliseconds>(callTimes[2] - callTimes[1]).count(),
			AllOf(Ge(100 - maxDiffInMs), Le(100 + maxDiffInMs))
		);
}

TEST_F(TimerTest, onTimerFixedDelayTiming) {
	const chrono::milliseconds::rep maxDiffInMs = 10;

	MockTimerCallback mockTimerCallback;
	EXPECT_CALL(mockTimerCallback, onTimer()).
			WillRepeatedly(Invoke(this, &TimerTest::saveCallTime));

	Timer timer(chrono::milliseconds(100), Timer::ScheduleType::FIXED_DELAY, "");

	timer.add(&mockTimerCallback);
	timer.start();
	this_thread::sleep_for(chrono::milliseconds(500));
	timer.stop();

	ASSERT_THAT(callTimes, SizeIs(3));

	EXPECT_THAT(chrono::duration_cast<chrono::milliseconds>(callTimes[1] - callTimes[0]).count(),
			AllOf(Ge(150 - maxDiffInMs), Le(150 + maxDiffInMs))
		);

	EXPECT_THAT(chrono::duration_cast<chrono::milliseconds>(callTimes[2] - callTimes[1]).count(),
			AllOf(Ge(150 - maxDiffInMs), Le(150 + maxDiffInMs))
		);
}

TEST_F(TimerTest, addDuplicate) {
	MockTimerCallback mockTimerCallback;
	Timer timer(chrono::milliseconds(100), Timer::ScheduleType::FIXED_RATE, "");

	EXPECT_NO_THROW(timer.add(&mockTimerCallback));
	EXPECT_THROW(timer.add(&mockTimerCallback), logic_error);
}

TEST_F(TimerTest, remove) {
	MockTimerCallback mockTimerCallback;
	EXPECT_CALL(mockTimerCallback, onTimer()).Times(1);

	Timer timer(chrono::milliseconds(100), Timer::ScheduleType::FIXED_RATE, "");

	timer.add(&mockTimerCallback);
	timer.start();
	this_thread::sleep_for(chrono::milliseconds(150));
	timer.remove(&mockTimerCallback);
	this_thread::sleep_for(chrono::milliseconds(150));
	timer.stop();
}

TEST_F(TimerTest, removeInvalid) {
	MockTimerCallback mockTimerCallback;
	Timer timer(chrono::milliseconds(100), Timer::ScheduleType::FIXED_RATE, "");

	EXPECT_THROW(timer.remove(&mockTimerCallback), logic_error);
}

TEST_F(TimerTest, removeAll) {
	MockTimerCallback mockTimerCallback;
	EXPECT_CALL(mockTimerCallback, onTimer()).Times(1);

	Timer timer(chrono::milliseconds(100), Timer::ScheduleType::FIXED_RATE, "");

	timer.add(&mockTimerCallback);
	timer.start();
	this_thread::sleep_for(chrono::milliseconds(150));
	timer.removeAll();
	this_thread::sleep_for(chrono::milliseconds(150));
	timer.stop();
}
*/
