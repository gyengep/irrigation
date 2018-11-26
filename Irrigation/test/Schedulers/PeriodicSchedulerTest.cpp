#include <gmock/gmock.h>
#include "Exceptions/Exceptions.h"
#include "Schedulers/PeriodicScheduler.h"
#include "Utils/ToTimeT.h"

using namespace std;
using namespace testing;


TEST(PeriodicSchedulerTest, constructor) {
	PeriodicScheduler scheduler;

	EXPECT_THAT(scheduler.getAdjustment(), 100);
	EXPECT_THAT(scheduler.getPeriod(), 1);
	EXPECT_FALSE(scheduler.isDayEnabled(0));
	EXPECT_THAT(scheduler.getPeriodStartYear(), 1970);
	EXPECT_THAT(scheduler.getPeriodStartMonth(), 1);
	EXPECT_THAT(scheduler.getPeriodStartDay(), 1);
}

TEST(PeriodicSchedulerTest, setPeriod) {
	PeriodicScheduler scheduler;

	scheduler.setPeriod(3);
	EXPECT_THAT(scheduler.getPeriod(), 3);
}

TEST(PeriodicSchedulerTest, setPeriodInvalid) {
	PeriodicScheduler scheduler;

	EXPECT_THROW(scheduler.setPeriod(0), ValueOutOfBoundsException);
	EXPECT_NO_THROW(scheduler.setPeriod(1));
	EXPECT_NO_THROW(scheduler.setPeriod(2));
	EXPECT_NO_THROW(scheduler.setPeriod(3));
	EXPECT_NO_THROW(scheduler.setPeriod(4));
	EXPECT_NO_THROW(scheduler.setPeriod(5));
	EXPECT_NO_THROW(scheduler.setPeriod(6));
	EXPECT_NO_THROW(scheduler.setPeriod(7));
	EXPECT_THROW(scheduler.setPeriod(8), ValueOutOfBoundsException);
}

TEST(PeriodicSchedulerTest, setPeriodStartDay) {
	PeriodicScheduler scheduler;
	const unsigned expectedPeriodStartYear = 2018;
	const unsigned expectedPeriodStartMonth = 10;
	const unsigned expectedPeriodStartDay = 20;

	scheduler.setPeriodStartDate(expectedPeriodStartYear, expectedPeriodStartMonth, expectedPeriodStartDay);
	EXPECT_THAT(scheduler.getPeriodStartYear(), Eq(expectedPeriodStartYear));
	EXPECT_THAT(scheduler.getPeriodStartMonth(), Eq(expectedPeriodStartMonth));
	EXPECT_THAT(scheduler.getPeriodStartDay(), Eq(expectedPeriodStartDay));
}

TEST(PeriodicSchedulerTest, setPeriodStartDayInvalid) {
	PeriodicScheduler scheduler;

	EXPECT_NO_THROW(scheduler.setPeriodStartDate(2018, 11, 15));
	EXPECT_THROW(scheduler.setPeriodStartDate(0, 11, 15), runtime_error);
	EXPECT_THROW(scheduler.setPeriodStartDate(2018, 0, 15), runtime_error);
	EXPECT_THROW(scheduler.setPeriodStartDate(2018, 13, 15), runtime_error);
	EXPECT_THROW(scheduler.setPeriodStartDate(2018, 11, 0), runtime_error);
	EXPECT_THROW(scheduler.setPeriodStartDate(2018, 11, 32), runtime_error);
}


TEST(PeriodicSchedulerTest, enableDay) {
	PeriodicScheduler scheduler;

	scheduler.setPeriod(5);

	scheduler.enableDay(0, true);
	scheduler.enableDay(1, false);
	scheduler.enableDay(2, false);
	scheduler.enableDay(3, false);
	scheduler.enableDay(4, false);

	EXPECT_TRUE(scheduler.isDayEnabled(0));
	EXPECT_FALSE(scheduler.isDayEnabled(1));
	EXPECT_FALSE(scheduler.isDayEnabled(2));
	EXPECT_FALSE(scheduler.isDayEnabled(3));
	EXPECT_FALSE(scheduler.isDayEnabled(4));

	scheduler.enableDay(3, true);

	EXPECT_TRUE(scheduler.isDayEnabled(0));
	EXPECT_FALSE(scheduler.isDayEnabled(1));
	EXPECT_FALSE(scheduler.isDayEnabled(2));
	EXPECT_TRUE(scheduler.isDayEnabled(3));
	EXPECT_FALSE(scheduler.isDayEnabled(4));

	scheduler.enableDay(0, false);

	EXPECT_FALSE(scheduler.isDayEnabled(0));
	EXPECT_FALSE(scheduler.isDayEnabled(1));
	EXPECT_FALSE(scheduler.isDayEnabled(2));
	EXPECT_TRUE(scheduler.isDayEnabled(3));
	EXPECT_FALSE(scheduler.isDayEnabled(4));
}

TEST(PeriodicSchedulerTest, enableDayInvalid) {
	PeriodicScheduler scheduler;

	scheduler.setPeriod(3);

	EXPECT_NO_THROW(scheduler.enableDay(0, true));
	EXPECT_NO_THROW(scheduler.enableDay(1, true));
	EXPECT_NO_THROW(scheduler.enableDay(2, true));
	EXPECT_THROW(scheduler.enableDay(3, true), IndexOutOfBoundsException);

	scheduler.setPeriod(2);

	EXPECT_NO_THROW(scheduler.enableDay(0, true));
	EXPECT_NO_THROW(scheduler.enableDay(1, true));
	EXPECT_THROW(scheduler.enableDay(2, true), IndexOutOfBoundsException);
}

TEST(PeriodicSchedulerTest, isDayEnabledInvalid) {
	PeriodicScheduler scheduler;

	scheduler.setPeriod(3);

	EXPECT_NO_THROW(scheduler.isDayEnabled(0));
	EXPECT_NO_THROW(scheduler.isDayEnabled(1));
	EXPECT_NO_THROW(scheduler.isDayEnabled(2));
	EXPECT_THROW(scheduler.isDayEnabled(3), IndexOutOfBoundsException);

	scheduler.setPeriod(2);

	EXPECT_NO_THROW(scheduler.isDayEnabled(0));
	EXPECT_NO_THROW(scheduler.isDayEnabled(1));
	EXPECT_THROW(scheduler.isDayEnabled(2), IndexOutOfBoundsException);
}

TEST(PeriodicSchedulerTest, isDayEnableAfterResize) {
	PeriodicScheduler scheduler;

	scheduler.setPeriod(2);

	scheduler.enableDay(0, true);
	scheduler.enableDay(1, true);

	EXPECT_TRUE(scheduler.isDayEnabled(0));
	EXPECT_TRUE(scheduler.isDayEnabled(1));

	scheduler.setPeriod(4);

	EXPECT_TRUE(scheduler.isDayEnabled(0));
	EXPECT_TRUE(scheduler.isDayEnabled(1));
	EXPECT_FALSE(scheduler.isDayEnabled(2));
	EXPECT_FALSE(scheduler.isDayEnabled(3));
}

TEST(PeriodicSchedulerTest, adjustment) {
	PeriodicScheduler scheduler;

	scheduler.setAdjustment(20);
	EXPECT_THAT(scheduler.getAdjustment(), 20);
}

TEST(PeriodicSchedulerTest, isDayScheduled) {
	PeriodicScheduler scheduler;

	scheduler.setPeriod(7);
	scheduler.setPeriodStartDate(2018, 11, 12);
	scheduler.enableDay(0, true);
	scheduler.enableDay(1, false);
	scheduler.enableDay(2, true);
	scheduler.enableDay(3, true);
	scheduler.enableDay(4, false);
	scheduler.enableDay(5, false);
	scheduler.enableDay(6, false);

	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 5)));
	EXPECT_FALSE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 6)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 7)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 8)));
	EXPECT_FALSE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 9)));
	EXPECT_FALSE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 10)));
	EXPECT_FALSE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 11)));

	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 12)));
	EXPECT_FALSE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 13)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 14)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 15)));
	EXPECT_FALSE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 16)));
	EXPECT_FALSE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 17)));
	EXPECT_FALSE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 18)));

	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 19)));
	EXPECT_FALSE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 20)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 21)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 22)));
	EXPECT_FALSE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 23)));
	EXPECT_FALSE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 24)));
	EXPECT_FALSE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 25)));

	///////////////////////////////////////////////////////////////////////////

	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2017, 11, 13)));
	EXPECT_FALSE(scheduler.isDayScheduled(toCalendarTime(2017, 11, 14)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2017, 11, 15)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2017, 11, 16)));
	EXPECT_FALSE(scheduler.isDayScheduled(toCalendarTime(2017, 11, 17)));
	EXPECT_FALSE(scheduler.isDayScheduled(toCalendarTime(2017, 11, 18)));
	EXPECT_FALSE(scheduler.isDayScheduled(toCalendarTime(2017, 11, 19)));

	///////////////////////////////////////////////////////////////////////////

	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2016, 11, 14)));
	EXPECT_FALSE(scheduler.isDayScheduled(toCalendarTime(2016, 11, 15)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2016, 11, 16)));
	EXPECT_TRUE(scheduler.isDayScheduled(toCalendarTime(2016, 11, 17)));
	EXPECT_FALSE(scheduler.isDayScheduled(toCalendarTime(2016, 11, 18)));
	EXPECT_FALSE(scheduler.isDayScheduled(toCalendarTime(2016, 11, 19)));
	EXPECT_FALSE(scheduler.isDayScheduled(toCalendarTime(2016, 11, 20)));
}

TEST(PeriodicSchedulerTest, getElapsedDaysSinceEpoch) {
	const int monthLengthsLeapYears[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	const int monthLengthsNotLeapYears[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	int elapsedDays = 0;

	for (int year = 1970; year < 2100; year++) {
		const int* monthLengths;

		if (0 == (year % 4)) {
			monthLengths = monthLengthsLeapYears;
		} else {
			monthLengths = monthLengthsNotLeapYears;
		}

		for (int month = 1; month <= 12; month++) {
			for (int dayOfMonth = 1; dayOfMonth <= monthLengths[month - 1]; dayOfMonth++) {
				const tm timeinfo = toCalendarTime(year, month, dayOfMonth);
				ASSERT_THAT(PeriodicScheduler::getElapsedDaysSinceEpoch(timeinfo), elapsedDays);
				elapsedDays++;
			}
		}
	}
}

TEST(PeriodicSchedulerTest, getElapsedDaysSinceEpoch2) {
	const tm timeinfo = toCalendarTime(2018, 11, 24);
	const int elapsedDays = PeriodicScheduler::getElapsedDaysSinceEpoch(timeinfo);

	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {
				const tm timeinfo = toCalendarTime(2018, 11, 24, hour, min, sec);
				ASSERT_THAT(PeriodicScheduler::getElapsedDaysSinceEpoch(timeinfo), elapsedDays);
			}
		}
	}
}

TEST(PeriodicSchedulerTest, getPeriodicSchedulerDTO) {
	PeriodicScheduler scheduler;

	scheduler.setAdjustment(123);
	scheduler.setPeriod(2);
	scheduler.enableDay(0, true);
	scheduler.enableDay(1, false);
	scheduler.setPeriodStartDate(2018, 11, 15);

	const PeriodicSchedulerDTO expectedSchedulerDTO(
			123,
			list<bool>({true, false }),
			2018, 11, 15
			);

	EXPECT_THAT(scheduler.getPeriodicSchedulerDTO(), Eq(expectedSchedulerDTO));
}

TEST(PeriodicSchedulerTest, updateFromDTO) {
	const PeriodicSchedulerDTO expectedSchedulerDTO(
		120,
		list<bool>({true, false, true, true}),
		2018, 12, 31
		);

	PeriodicScheduler scheduler;
	scheduler.updateFromDTO(expectedSchedulerDTO);

	EXPECT_THAT(scheduler.getAdjustment(), 120);
	EXPECT_TRUE(scheduler.isDayEnabled(0));
	EXPECT_FALSE(scheduler.isDayEnabled(1));
	EXPECT_TRUE(scheduler.isDayEnabled(2));
	EXPECT_TRUE(scheduler.isDayEnabled(3));
	EXPECT_THAT(scheduler.getPeriod(), 4);

	EXPECT_THAT(scheduler.getPeriodicSchedulerDTO(), Eq(expectedSchedulerDTO));
}
