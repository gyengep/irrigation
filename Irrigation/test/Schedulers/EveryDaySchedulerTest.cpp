#include <gmock/gmock.h>
#include "Schedulers/EveryDayScheduler.h"
#include "Utils/TimeConversion.h"
#include "Dto2Object/EveryDaySchedulerSamples.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;


time_t toLocalTime(int year, int month, int day) {
	struct tm timeinfo = toCalendarTime(year, month, day);
	return timelocal(&timeinfo);
}

TEST(EveryDaySchedulerTest, isDayScheduled) {
	EveryDayScheduler scheduler;

	EXPECT_THAT(scheduler.process(toLocalTime(2018, 11, 5)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2018, 11, 6)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2018, 11, 7)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2018, 11, 8)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2018, 11, 9)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2018, 11, 10)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2018, 11, 11)), Eq(Scheduler::Result(true, false, 0)));

	EXPECT_THAT(scheduler.process(toLocalTime(2018, 11, 12)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2018, 11, 13)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2018, 11, 14)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2018, 11, 15)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2018, 11, 16)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2018, 11, 17)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2018, 11, 18)), Eq(Scheduler::Result(true, false, 0)));

	EXPECT_THAT(scheduler.process(toLocalTime(2018, 11, 19)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2018, 11, 20)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2018, 11, 21)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2018, 11, 22)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2018, 11, 23)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2018, 11, 24)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2018, 11, 25)), Eq(Scheduler::Result(true, false, 0)));

	///////////////////////////////////////////////////////////////////////////

	EXPECT_THAT(scheduler.process(toLocalTime(2017, 11, 13)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2017, 11, 14)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2017, 11, 15)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2017, 11, 16)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2017, 11, 17)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2017, 11, 18)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2017, 11, 19)), Eq(Scheduler::Result(true, false, 0)));

	///////////////////////////////////////////////////////////////////////////

	EXPECT_THAT(scheduler.process(toLocalTime(2016, 11, 14)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2016, 11, 15)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2016, 11, 16)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2016, 11, 17)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2016, 11, 18)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2016, 11, 19)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(toLocalTime(2016, 11, 20)), Eq(Scheduler::Result(true, false, 0)));
}

///////////////////////////////////////////////////////////////////////////////

void testToEveryDaySchedulerDto(const EveryDaySchedulerSample& everyDaySchedulerSample) {
	const shared_ptr<EveryDayScheduler> everyDayScheduler = everyDaySchedulerSample.getObject();
	const EveryDaySchedulerDTO& expectedEveryDaySchedulerDto = everyDaySchedulerSample.getDto();
	EXPECT_THAT(everyDayScheduler->toEveryDaySchedulerDto(), Eq(expectedEveryDaySchedulerDto));
}

TEST(EveryDaySchedulerTest, toEveryDaySchedulerDto) {
	testToEveryDaySchedulerDto(EveryDaySchedulerSample1());
}

///////////////////////////////////////////////////////////////////////////////

void testUpdateFromEveryDaySchedulerDto(shared_ptr<EveryDayScheduler> everyDayScheduler, const EveryDaySchedulerSample& everyDaySchedulerSample) {
	//EXPECT_THAT(everyDayScheduler, Not(Pointee(*everyDaySchedulerSample.getObject())));
	everyDayScheduler->updateFromEveryDaySchedulerDto(everyDaySchedulerSample.getDto());
	EXPECT_THAT(everyDayScheduler, Pointee(*everyDaySchedulerSample.getObject()));
}

TEST(EveryDaySchedulerTest, updateFromEveryDaySchedulerDto1) {
	shared_ptr<EveryDayScheduler> everyDayScheduler = shared_ptr<EveryDayScheduler>(new EveryDayScheduler());
	testUpdateFromEveryDaySchedulerDto(everyDayScheduler, EveryDaySchedulerSample1());
}
