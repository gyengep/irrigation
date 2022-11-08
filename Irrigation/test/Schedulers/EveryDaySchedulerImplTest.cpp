#include <gmock/gmock.h>
#include "Schedulers/EveryDaySchedulerImpl.h"
#include "Utils/TimeConversion.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(EveryDaySchedulerImplTest, isDayScheduled) {
	EveryDaySchedulerImpl scheduler;

	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 5)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 6)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 7)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 8)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 9)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 10)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 11)), Eq(Scheduler::Result(true, false, 0)));

	///////////////////////////////////////////////////////////////////////////

	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 12)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 13)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 14)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 15)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 16)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 17)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 18)), Eq(Scheduler::Result(true, false, 0)));

	///////////////////////////////////////////////////////////////////////////

	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 19)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 20)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 21)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 22)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 23)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 24)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 25)), Eq(Scheduler::Result(true, false, 0)));

	///////////////////////////////////////////////////////////////////////////

	EXPECT_THAT(scheduler.process(LocalDateTime(2017, 11, 13)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2017, 11, 14)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2017, 11, 15)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2017, 11, 16)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2017, 11, 17)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2017, 11, 18)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2017, 11, 19)), Eq(Scheduler::Result(true, false, 0)));

	///////////////////////////////////////////////////////////////////////////

	EXPECT_THAT(scheduler.process(LocalDateTime(2016, 11, 14)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2016, 11, 15)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2016, 11, 16)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2016, 11, 17)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2016, 11, 18)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2016, 11, 19)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2016, 11, 20)), Eq(Scheduler::Result(true, false, 0)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(EveryDaySchedulerImplTest, toEveryDaySchedulerDto) {
	EXPECT_THAT(EveryDaySchedulerImpl().toEveryDaySchedulerDto(), Eq(EveryDaySchedulerDTO()));
}

///////////////////////////////////////////////////////////////////////////////

TEST(EveryDaySchedulerImplTest, updateFromDto) {
	EveryDaySchedulerImpl().updateFromEveryDaySchedulerDto(EveryDaySchedulerDTO());
}
