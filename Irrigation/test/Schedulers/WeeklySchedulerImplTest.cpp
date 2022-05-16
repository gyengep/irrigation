#include <gmock/gmock.h>
#include <list>
#include "Exceptions/Exceptions.h"
#include "Schedulers/WeeklySchedulerImpl.h"
#include "Utils/TimeConversion.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void checkDay(std::tm& timeinfo, Scheduler& scheduler, bool expectedResult) {
	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {

				timeinfo.tm_hour = hour;
				timeinfo.tm_min = min;
				timeinfo.tm_sec = sec;

				ASSERT_THAT(scheduler.process(mktime(&timeinfo)), Eq(Scheduler::Result(expectedResult, false, 0)));
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(WeeklySchedulerImplTest, defaultConstructor) {
	WeeklySchedulerImpl scheduler;

	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::MONDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::TUESDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::WEDNESDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::THURSDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::FRIDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::SATURDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::SUNDAY));
}

TEST(WeeklySchedulerImplTest, initializerConstructor) {
	WeeklySchedulerImpl scheduler({true, false, true, true, false, false, false});

	EXPECT_TRUE(scheduler.isDayEnabled(0));
	EXPECT_FALSE(scheduler.isDayEnabled(1));
	EXPECT_TRUE(scheduler.isDayEnabled(2));
	EXPECT_TRUE(scheduler.isDayEnabled(3));
	EXPECT_FALSE(scheduler.isDayEnabled(4));
	EXPECT_FALSE(scheduler.isDayEnabled(5));
	EXPECT_FALSE(scheduler.isDayEnabled(6));
}

TEST(WeeklySchedulerImplTest, equalsOperator) {
	WeeklySchedulerImpl scheduler1;
	WeeklySchedulerImpl scheduler2;

	EXPECT_TRUE(scheduler1 == scheduler2);
	EXPECT_TRUE(scheduler2 == scheduler1);

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

TEST(WeeklySchedulerImplTest, enableDay) {
	WeeklySchedulerImpl scheduler;

	scheduler.enableDay(WeeklySchedulerImpl::WEDNESDAY, true);

	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::MONDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::TUESDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklySchedulerImpl::WEDNESDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::THURSDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::FRIDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::SATURDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::SUNDAY));

	scheduler.enableDay(WeeklySchedulerImpl::FRIDAY, true);

	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::MONDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::TUESDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklySchedulerImpl::WEDNESDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::THURSDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklySchedulerImpl::FRIDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::SATURDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::SUNDAY));

	scheduler.enableDay(WeeklySchedulerImpl::WEDNESDAY, false);

	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::MONDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::TUESDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::WEDNESDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::THURSDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklySchedulerImpl::FRIDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::SATURDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::SUNDAY));
}

TEST(WeeklySchedulerImplTest, enableDayInvalid) {
	WeeklySchedulerImpl scheduler;
	EXPECT_THROW(scheduler.enableDay(7, true), IndexOutOfBoundsException);
}

TEST(WeeklySchedulerImplTest, isDayEnabledInvalid) {
	WeeklySchedulerImpl scheduler;
	EXPECT_THROW(scheduler.isDayEnabled(7), IndexOutOfBoundsException);
}

TEST(WeeklySchedulerImplTest, isDayScheduled) {
	WeeklySchedulerImpl scheduler;

	scheduler.enableDay(WeeklySchedulerImpl::WEDNESDAY, true);

	for (int day = 19; day < 26; day++) {
		tm timeinfo = toCalendarTime(2018, 11, day);
		timeinfo.tm_wday = (day-18) % 7;
		checkDay(timeinfo, scheduler, 21 == day);
	}

	scheduler.enableDay(WeeklySchedulerImpl::WEDNESDAY, false);
	scheduler.enableDay(WeeklySchedulerImpl::THURSDAY, true);
	scheduler.enableDay(WeeklySchedulerImpl::SUNDAY, true);

	for (int day = 19; day < 26; day++) {
		tm timeinfo = toCalendarTime(2018, 11, day);
		timeinfo.tm_wday = (day-18) % 7;
		checkDay(timeinfo, scheduler, 22 == day || 25 == day);
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(WeeklySchedulerImplTest, toWeeklySchedulerDto) {
	EXPECT_THAT(
		WeeklySchedulerImpl({ false, false, false, false, false, false, false }).toWeeklySchedulerDto(),
		Eq(WeeklySchedulerDTO(std::list<bool> { false, false, false, false, false, false, false }))
	);

	EXPECT_THAT(
		WeeklySchedulerImpl({ true, true, true, true, true, true, true }).toWeeklySchedulerDto(),
		Eq(WeeklySchedulerDTO(std::list<bool> { true, true, true, true, true, true, true }))
	);

	EXPECT_THAT(
		WeeklySchedulerImpl({ true, false, false, false, false, false, false }).toWeeklySchedulerDto(),
		Eq(WeeklySchedulerDTO(std::list<bool> { true, false, false, false, false, false, false }))
	);

	EXPECT_THAT(
		WeeklySchedulerImpl({ false, true, true, false, false, true, true }).toWeeklySchedulerDto(),
		Eq(WeeklySchedulerDTO(std::list<bool> { false, true, true, false, false, true, true }))
	);
}

///////////////////////////////////////////////////////////////////////////////

TEST(WeeklySchedulerImplTest, partialUpdateFromWeeklySchedulerDto_empty) {
	const WeeklySchedulerImpl expected({ true, true, true, false, false, false, false });
	WeeklySchedulerImpl actual({ true, true, true, false, false, false, false });

	actual.updateFromWeeklySchedulerDto(WeeklySchedulerDTO());

	EXPECT_THAT(actual, Eq(expected));
}

TEST(WeeklySchedulerImplTest, partialUpdateFromWeeklySchedulerDto_days) {
	const WeeklySchedulerImpl expected1 { false, false, true, true, false, true, false};
	const WeeklySchedulerImpl expected2 { true, true, true, true, false, false, false};

	WeeklySchedulerImpl actual;

	actual.updateFromWeeklySchedulerDto(WeeklySchedulerDTO(std::list<bool> { false, false, true, true, false, true, false }));
	EXPECT_THAT(actual, Eq(expected1));

	actual.updateFromWeeklySchedulerDto(WeeklySchedulerDTO(std::list<bool> { true, true, true, true, false, false, false }));
	EXPECT_THAT(actual, Eq(expected2));
}

