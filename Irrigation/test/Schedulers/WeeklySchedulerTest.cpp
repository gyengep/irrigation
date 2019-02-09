#include <gmock/gmock.h>
#include <list>
#include "Exceptions/Exceptions.h"
#include "Schedulers/WeeklyScheduler.h"
#include "Utils/TimeConversion.h"
#include "Dto2Object/WeeklySchedulerSamples.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;

///////////////////////////////////////////////////////////////////////////////

void checkDay(tm& timeinfo, const Scheduler& scheduler, bool expectedResult) {
	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {
				timeinfo.tm_hour = hour;
				timeinfo.tm_min = min;
				timeinfo.tm_sec = sec;
				ASSERT_THAT(scheduler.isDayScheduled(timeinfo), Eq(expectedResult));
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(WeeklySchedulerTest, defaultConstructor) {
	WeeklyScheduler scheduler;

	EXPECT_THAT(scheduler.getAdjustment(), Eq(100));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::MONDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::TUESDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::WEDNESDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::THURSDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::FRIDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::SATURDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::SUNDAY));
}

TEST(WeeklySchedulerTest, parametrizedConstructor) {
	WeeklyScheduler scheduler(150, array<bool, 7>({true, false, true, true, false, false, false}));

	EXPECT_THAT(scheduler.getAdjustment(), Eq(150));
	EXPECT_TRUE(scheduler.isDayEnabled(0));
	EXPECT_FALSE(scheduler.isDayEnabled(1));
	EXPECT_TRUE(scheduler.isDayEnabled(2));
	EXPECT_TRUE(scheduler.isDayEnabled(3));
	EXPECT_FALSE(scheduler.isDayEnabled(4));
	EXPECT_FALSE(scheduler.isDayEnabled(5));
	EXPECT_FALSE(scheduler.isDayEnabled(6));
}

TEST(WeeklySchedulerTest, copyConstructor) {
	WeeklyScheduler scheduler(WeeklyScheduler(150, array<bool, 7>({true, false, true, true, false, false, false})));

	EXPECT_THAT(scheduler.getAdjustment(), Eq(150));
	EXPECT_TRUE(scheduler.isDayEnabled(0));
	EXPECT_FALSE(scheduler.isDayEnabled(1));
	EXPECT_TRUE(scheduler.isDayEnabled(2));
	EXPECT_TRUE(scheduler.isDayEnabled(3));
	EXPECT_FALSE(scheduler.isDayEnabled(4));
	EXPECT_FALSE(scheduler.isDayEnabled(5));
	EXPECT_FALSE(scheduler.isDayEnabled(6));
}

TEST(WeeklySchedulerTest, equalsOperator) {
	WeeklyScheduler scheduler1;
	WeeklyScheduler scheduler2;

	EXPECT_TRUE(scheduler1 == scheduler2);
	EXPECT_TRUE(scheduler2 == scheduler1);

	{
		const unsigned adjustment1 = 80;
		const unsigned adjustment2 = 110;

		scheduler1.setAdjustment(adjustment1);
		EXPECT_FALSE(scheduler1 == scheduler2);
		EXPECT_FALSE(scheduler2 == scheduler1);

		scheduler2.setAdjustment(adjustment2);
		EXPECT_FALSE(scheduler1 == scheduler2);
		EXPECT_FALSE(scheduler2 == scheduler1);

		scheduler1.setAdjustment(adjustment2);
		EXPECT_TRUE(scheduler1 == scheduler2);
		EXPECT_TRUE(scheduler2 == scheduler1);
	}

	{
		scheduler1.enableDay(1, true);
		EXPECT_FALSE(scheduler1 == scheduler2);
		EXPECT_FALSE(scheduler2 == scheduler1);

		scheduler2.enableDay(2, true);
		EXPECT_FALSE(scheduler1 == scheduler2);
		EXPECT_FALSE(scheduler2 == scheduler1);

		scheduler1.enableDay(1, false);
		EXPECT_FALSE(scheduler1 == scheduler2);
		EXPECT_FALSE(scheduler2 == scheduler1);

		scheduler1.enableDay(2, true);
		EXPECT_TRUE(scheduler1 == scheduler2);
		EXPECT_TRUE(scheduler2 == scheduler1);
	}
}

TEST(WeeklySchedulerTest, setAdjustment) {
	WeeklyScheduler scheduler;

	scheduler.setAdjustment(53);
	EXPECT_THAT(scheduler.getAdjustment(), Eq(53));
}

TEST(WeeklySchedulerTest, enableDay) {
	WeeklyScheduler scheduler;

	scheduler.enableDay(WeeklyScheduler::WEDNESDAY, true);

	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::MONDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::TUESDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklyScheduler::WEDNESDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::THURSDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::FRIDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::SATURDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::SUNDAY));

	scheduler.enableDay(WeeklyScheduler::FRIDAY, true);

	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::MONDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::TUESDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklyScheduler::WEDNESDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::THURSDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklyScheduler::FRIDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::SATURDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::SUNDAY));

	scheduler.enableDay(WeeklyScheduler::WEDNESDAY, false);

	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::MONDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::TUESDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::WEDNESDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::THURSDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklyScheduler::FRIDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::SATURDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::SUNDAY));
}

TEST(WeeklySchedulerTest, enableDayInvalid) {
	WeeklyScheduler scheduler;
	EXPECT_THROW(scheduler.enableDay(7, true), IndexOutOfBoundsException);
}

TEST(WeeklySchedulerTest, isDayEnabledInvalid) {
	WeeklyScheduler scheduler;
	EXPECT_THROW(scheduler.isDayEnabled(7), IndexOutOfBoundsException);
}

TEST(WeeklySchedulerTest, isDayScheduled) {
	WeeklyScheduler scheduler;

	scheduler.enableDay(WeeklyScheduler::WEDNESDAY, true);

	for (int day = 19; day < 26; day++) {
		tm timeinfo = toCalendarTime(2018, 11, day);
		timeinfo.tm_wday = (day-18) % 7;
		checkDay(timeinfo, scheduler, 21 == day);
	}

	scheduler.enableDay(WeeklyScheduler::WEDNESDAY, false);
	scheduler.enableDay(WeeklyScheduler::THURSDAY, true);
	scheduler.enableDay(WeeklyScheduler::SUNDAY, true);

	for (int day = 19; day < 26; day++) {
		tm timeinfo = toCalendarTime(2018, 11, day);
		timeinfo.tm_wday = (day-18) % 7;
		checkDay(timeinfo, scheduler, 22 == day || 25 == day);
	}
}

///////////////////////////////////////////////////////////////////////////////

void testToWeeklySchedulerDto(const WeeklySchedulerSample& weeklySchedulerSample) {
	EXPECT_THAT(weeklySchedulerSample.getObject()->toWeeklySchedulerDto(), Eq(weeklySchedulerSample.getDto()));
}

TEST(WeeklySchedulerTest, toWeeklySchedulerDto1) {
	testToWeeklySchedulerDto(WeeklySchedulerSample1());
}

TEST(WeeklySchedulerTest, toWeeklySchedulerDto2) {
	testToWeeklySchedulerDto(WeeklySchedulerSample2());
}

TEST(WeeklySchedulerTest, toWeeklySchedulerDto3) {
	testToWeeklySchedulerDto(WeeklySchedulerSample3());
}

///////////////////////////////////////////////////////////////////////////////

void testUpdateFromWeeklySchedulerDto(shared_ptr<WeeklyScheduler> weeklyScheduler, const WeeklySchedulerSample& weeklySchedulerSample) {
	EXPECT_THAT(weeklyScheduler, Not(Pointee(*weeklySchedulerSample.getObject())));
	weeklyScheduler->updateFromWeeklySchedulerDto(weeklySchedulerSample.getDto());
	EXPECT_THAT(weeklyScheduler, Pointee(*weeklySchedulerSample.getObject()));
}

TEST(WeeklySchedulerTest, updateFromWeeklySchedulerDto1) {
	shared_ptr<WeeklyScheduler> weeklyScheduler = shared_ptr<WeeklyScheduler>(new WeeklyScheduler());
	testUpdateFromWeeklySchedulerDto(weeklyScheduler, WeeklySchedulerSample1());
}

TEST(WeeklySchedulerTest, updateFromWeeklySchedulerDto2) {
	shared_ptr<WeeklyScheduler> weeklyScheduler = WeeklySchedulerSample1().getObject();
	testUpdateFromWeeklySchedulerDto(weeklyScheduler, WeeklySchedulerSample2());
}

TEST(WeeklySchedulerTest, updateFromWeeklySchedulerDto3) {
	shared_ptr<WeeklyScheduler> weeklyScheduler = WeeklySchedulerSample2().getObject();
	testUpdateFromWeeklySchedulerDto(weeklyScheduler, WeeklySchedulerSample3());
}

TEST(WeeklySchedulerTest, updateFromWeeklySchedulerDto4) {
	shared_ptr<WeeklyScheduler> weeklyScheduler = WeeklySchedulerSample3().getObject();
	testUpdateFromWeeklySchedulerDto(weeklyScheduler, WeeklySchedulerSample4());
}

///////////////////////////////////////////////////////////////////////////////

TEST(WeeklySchedulerTest, partialUpdateFromWeeklySchedulerDto_empty) {
	WeeklyScheduler actualWeeklyScheduler(*WeeklySchedulerSample1().getObject());
	WeeklyScheduler expectedWeeklyScheduler(*WeeklySchedulerSample1().getObject());

	actualWeeklyScheduler.updateFromWeeklySchedulerDto(WeeklySchedulerDTO());

	EXPECT_THAT(actualWeeklyScheduler, Eq(expectedWeeklyScheduler));
}

TEST(WeeklySchedulerTest, partialUpdateFromWeeklySchedulerDto_adjustment) {
	const unsigned adjustment1 = 37;
	const unsigned adjustment2 = 92;

	WeeklyScheduler actualWeeklyScheduler(*WeeklySchedulerSample2().getObject());
	WeeklyScheduler expectedWeeklyScheduler(*WeeklySchedulerSample2().getObject());

	actualWeeklyScheduler.updateFromWeeklySchedulerDto(WeeklySchedulerDTO().setAdjustment(adjustment1));
	expectedWeeklyScheduler.setAdjustment(adjustment1);
	EXPECT_THAT(actualWeeklyScheduler, Eq(expectedWeeklyScheduler));

	actualWeeklyScheduler.updateFromWeeklySchedulerDto(WeeklySchedulerDTO().setAdjustment(adjustment2));
	expectedWeeklyScheduler.setAdjustment(adjustment2);
	EXPECT_THAT(actualWeeklyScheduler, Eq(expectedWeeklyScheduler));
}

TEST(WeeklySchedulerTest, partialUpdateFromWeeklySchedulerDto_days) {
	const list<bool> days1({ false, false, true, true, false, true, false});
	const list<bool> days2({ true, true, true, true, false, false, false});

	WeeklyScheduler actualWeeklyScheduler(*WeeklySchedulerSample3().getObject());
	WeeklyScheduler expectedWeeklyScheduler(*WeeklySchedulerSample3().getObject());

	actualWeeklyScheduler.updateFromWeeklySchedulerDto(WeeklySchedulerDTO().setValues(list<bool>(days1)));
	for (size_t i = 0; i < days1.size(); ++i) {
		expectedWeeklyScheduler.enableDay(i, *next(days1.begin(), i));
	}
	EXPECT_THAT(actualWeeklyScheduler, Eq(expectedWeeklyScheduler));

	actualWeeklyScheduler.updateFromWeeklySchedulerDto(WeeklySchedulerDTO().setValues(list<bool>(days2)));
	for (size_t i = 0; i < days2.size(); ++i) {
		expectedWeeklyScheduler.enableDay(i, *next(days2.begin(), i));
	}
	EXPECT_THAT(actualWeeklyScheduler, Eq(expectedWeeklyScheduler));
}

