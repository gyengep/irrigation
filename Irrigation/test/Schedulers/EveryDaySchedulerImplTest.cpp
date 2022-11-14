#include <gmock/gmock.h>
#include "Schedulers/EveryDaySchedulerImpl.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(EveryDaySchedulerImplTest, isDayScheduled) {
	EveryDaySchedulerImpl scheduler;

	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 5)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 6)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 7)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 8)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 9)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 10)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 11)), Pointee(Scheduler::Result(true)));

	///////////////////////////////////////////////////////////////////////////

	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 12)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 13)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 14)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 15)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 16)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 17)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 18)), Pointee(Scheduler::Result(true)));

	///////////////////////////////////////////////////////////////////////////

	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 19)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 20)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 21)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 22)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 23)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 24)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2018, 11, 25)), Pointee(Scheduler::Result(true)));

	///////////////////////////////////////////////////////////////////////////

	EXPECT_THAT(scheduler.process(LocalDateTime(2017, 11, 13)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2017, 11, 14)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2017, 11, 15)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2017, 11, 16)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2017, 11, 17)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2017, 11, 18)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2017, 11, 19)), Pointee(Scheduler::Result(true)));

	///////////////////////////////////////////////////////////////////////////

	EXPECT_THAT(scheduler.process(LocalDateTime(2016, 11, 14)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2016, 11, 15)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2016, 11, 16)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2016, 11, 17)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2016, 11, 18)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2016, 11, 19)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(scheduler.process(LocalDateTime(2016, 11, 20)), Pointee(Scheduler::Result(true)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(EveryDaySchedulerImplTest, toEveryDaySchedulerDto) {
	EXPECT_THAT(EveryDaySchedulerImpl().toEveryDaySchedulerDto(), Eq(EveryDaySchedulerDTO()));
}

///////////////////////////////////////////////////////////////////////////////

TEST(EveryDaySchedulerImplTest, updateFromDto) {
	EveryDaySchedulerImpl().updateFromEveryDaySchedulerDto(EveryDaySchedulerDTO());
}
