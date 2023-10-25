#include "schedulers/Impl/EveryDaySchedulerImpl.h"
#include "Dto2ObjectSamples/EveryDaySchedulerSamples.h"
#include <gmock/gmock.h>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(EveryDaySchedulerImplFactoryTest, create) {
	EXPECT_THAT(EveryDaySchedulerImplFactory().create(), Not(IsNull()));
	EXPECT_TRUE(EveryDaySchedulerImplFactory().create() != EveryDaySchedulerImplFactory().create());
}

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

TEST(EveryDaySchedulerImplToDtoTest, toEveryDaySchedulerDto) {
	const Dto2ObjectTestSamples::EveryDaySchedulerSample sample;
	const EveryDaySchedulerImpl& actual = sample.getObject();
	const EveryDaySchedulerDto& expected = sample.getDto();

	EXPECT_THAT(actual.toEveryDaySchedulerDto(), Eq(expected));
}

TEST(EveryDaySchedulerImplFromDtoTest, updateFromEveryDaySchedulerDto) {
	EveryDaySchedulerImpl actualEveryDayScheduler;
	const EveryDaySchedulerDto actualEveryDaySchedulerDto = Dto2ObjectTestSamples::EveryDaySchedulerSample().getDto();
	actualEveryDayScheduler.updateFromEveryDaySchedulerDto(actualEveryDaySchedulerDto);
}
