#include <gmock/gmock.h>
#include "Schedulers/EveryDayScheduler.h"
#include "Utils/TimeConversion.h"
#include "Dto2Object/EveryDaySchedulerSamples.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;


TEST(EveryDaySchedulerTest, isDayScheduled) {
	EveryDayScheduler scheduler;

	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 5)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 6)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 7)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 8)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 9)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 10)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 11)));

	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 12)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 13)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 14)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 15)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 16)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 17)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 18)));

	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 19)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 20)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 21)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 22)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 23)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 24)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 25)));

	///////////////////////////////////////////////////////////////////////////

	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2017, 11, 13)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2017, 11, 14)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2017, 11, 15)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2017, 11, 16)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2017, 11, 17)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2017, 11, 18)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2017, 11, 19)));

	///////////////////////////////////////////////////////////////////////////

	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2016, 11, 14)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2016, 11, 15)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2016, 11, 16)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2016, 11, 17)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2016, 11, 18)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2016, 11, 19)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2016, 11, 20)));
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
