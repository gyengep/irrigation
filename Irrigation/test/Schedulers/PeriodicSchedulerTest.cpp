#include <gmock/gmock.h>
#include "Exceptions/Exceptions.h"
#include "Schedulers/PeriodicScheduler.h"
#include "Utils/TimeConversion.h"
#include "PeriodicSchedulerSamples.h"

using namespace std;
using namespace testing;
using namespace LogicTest;


TEST(PeriodicSchedulerTest, constructor) {
	PeriodicScheduler scheduler;

	EXPECT_THAT(scheduler.getAdjustment(), 100);
	EXPECT_THAT(scheduler.getPeriod(), 0);
	EXPECT_THAT(scheduler.getPeriodStartYear(), 1970);
	EXPECT_THAT(scheduler.getPeriodStartMonth(), 1);
	EXPECT_THAT(scheduler.getPeriodStartDay(), 1);
}

TEST(PeriodicSchedulerTest, setPeriod) {
	PeriodicScheduler scheduler;

	scheduler.setPeriod(3);
	EXPECT_THAT(scheduler.getPeriod(), 3);

	EXPECT_FALSE(scheduler.isDayEnabled(0));
	EXPECT_FALSE(scheduler.isDayEnabled(1));
	EXPECT_FALSE(scheduler.isDayEnabled(2));
}

TEST(PeriodicSchedulerTest, setPeriodInvalid) {
	PeriodicScheduler scheduler;

	EXPECT_NO_THROW(scheduler.setPeriod(0));
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

	EXPECT_THROW(scheduler.isDayEnabled(0), IndexOutOfBoundsException);

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

TEST(PeriodicSchedulerTest, isDayScheduledInvalid) {
	PeriodicScheduler scheduler;

	scheduler.setPeriod(0);
	EXPECT_FALSE(scheduler.isDayScheduled(toCalendarTime(2018, 11, 5)));
}

///////////////////////////////////////////////////////////////////////////////

void testToPeriodicSchedulerDto(const PeriodicSchedulerSample& periodicSchedulerSample) {
	const shared_ptr<PeriodicScheduler> periodicScheduler = periodicSchedulerSample.getPeriodicScheduler();
	const PeriodicSchedulerDTO& expectedPeriodicSchedulerDto = periodicSchedulerSample.getPeriodicSchedulerDto();
	EXPECT_THAT(periodicScheduler->toPeriodicSchedulerDto(), Eq(expectedPeriodicSchedulerDto));
}

TEST(PeriodicSchedulerTest, toPeriodicSchedulerDto1) {
	testToPeriodicSchedulerDto(PeriodicSchedulerSample1());
}

TEST(PeriodicSchedulerTest, toPeriodicSchedulerDto2) {
	testToPeriodicSchedulerDto(PeriodicSchedulerSample2());
}

TEST(PeriodicSchedulerTest, toPeriodicSchedulerDto3) {
	testToPeriodicSchedulerDto(PeriodicSchedulerSample3());
}

///////////////////////////////////////////////////////////////////////////////

void testUpdateFromPeriodicSchedulerDto(shared_ptr<PeriodicScheduler> periodicScheduler, const PeriodicSchedulerSample& periodicSchedulerSample) {
	EXPECT_THAT(periodicScheduler, Not(Pointee(*periodicSchedulerSample.getPeriodicScheduler())));
	periodicScheduler->updateFromPeriodicSchedulerDto(periodicSchedulerSample.getPeriodicSchedulerDto());
	EXPECT_THAT(periodicScheduler, Pointee(*periodicSchedulerSample.getPeriodicScheduler()));
}

TEST(PeriodicSchedulerTest, updateFromPeriodicSchedulerDto1) {
	shared_ptr<PeriodicScheduler> periodicScheduler = shared_ptr<PeriodicScheduler>(new PeriodicScheduler());
	testUpdateFromPeriodicSchedulerDto(periodicScheduler, PeriodicSchedulerSample1());
}

TEST(PeriodicSchedulerTest, updateFromPeriodicSchedulerDto2) {
	shared_ptr<PeriodicScheduler> periodicScheduler = PeriodicSchedulerSample1().getPeriodicScheduler();
	testUpdateFromPeriodicSchedulerDto(periodicScheduler, PeriodicSchedulerSample2());
}

TEST(PeriodicSchedulerTest, updateFromPeriodicSchedulerDto3) {
	shared_ptr<PeriodicScheduler> periodicScheduler = PeriodicSchedulerSample2().getPeriodicScheduler();
	testUpdateFromPeriodicSchedulerDto(periodicScheduler, PeriodicSchedulerSample3());
}

TEST(PeriodicSchedulerTest, updateFromPeriodicSchedulerDto4) {
	shared_ptr<PeriodicScheduler> periodicScheduler = PeriodicSchedulerSample3().getPeriodicScheduler();
	testUpdateFromPeriodicSchedulerDto(periodicScheduler, PeriodicSchedulerSample4());
}

///////////////////////////////////////////////////////////////////////////////

TEST(PeriodicSchedulerTest, partialUpdateFromPeriodicSchedulerDto_empty) {
	const PeriodicScheduler periodicScheduler(*PeriodicSchedulerSample1().getPeriodicScheduler());

	PeriodicScheduler actualPeriodicScheduler(periodicScheduler);
	actualPeriodicScheduler.updateFromPeriodicSchedulerDto(PeriodicSchedulerDTO());

	PeriodicScheduler expectedPeriodicScheduler(periodicScheduler);

	EXPECT_THAT(actualPeriodicScheduler, Eq(expectedPeriodicScheduler));
}

TEST(PeriodicSchedulerTest, partialUpdateFromPeriodicSchedulerDto_adjustment) {
	const PeriodicScheduler periodicScheduler(*PeriodicSchedulerSample2().getPeriodicScheduler());
	const unsigned adjustment = 80;

	PeriodicScheduler actualPeriodicScheduler(periodicScheduler);
	actualPeriodicScheduler.updateFromPeriodicSchedulerDto(PeriodicSchedulerDTO().setAdjustment(adjustment));

	PeriodicScheduler expectedPeriodicScheduler(periodicScheduler);
	expectedPeriodicScheduler.setAdjustment(adjustment);


	EXPECT_THAT(actualPeriodicScheduler, Eq(expectedPeriodicScheduler));
}

TEST(PeriodicSchedulerTest, partialUpdateFromPeriodicSchedulerDto_days) {
	const PeriodicScheduler periodicScheduler(*PeriodicSchedulerSample3().getPeriodicScheduler());
	const list<bool> days({ true, false });

	PeriodicScheduler actualPeriodicScheduler(periodicScheduler);
	actualPeriodicScheduler.updateFromPeriodicSchedulerDto(PeriodicSchedulerDTO().setValues(list<bool>(days)));

	PeriodicScheduler expectedPeriodicScheduler(periodicScheduler);
	expectedPeriodicScheduler.setPeriod(days.size());
	for (size_t i = 0; i < days.size(); ++i) {
		expectedPeriodicScheduler.enableDay(i, *next(days.begin(), i));
	}

	EXPECT_THAT(actualPeriodicScheduler, Eq(expectedPeriodicScheduler));
}

TEST(PeriodicSchedulerTest, partialUpdateFromPeriodicSchedulerDto_periodStart) {
	const PeriodicScheduler periodicScheduler(*PeriodicSchedulerSample4().getPeriodicScheduler());
	const unsigned year = 1999;
	const unsigned month = 11;
	const unsigned day = 25;

	PeriodicScheduler actualPeriodicScheduler(periodicScheduler);
	actualPeriodicScheduler.updateFromPeriodicSchedulerDto(
			PeriodicSchedulerDTO().
			setPeriodStartYear(year).
			setPeriodStartMonth(month).
			setPeriodStartDay(day));

	PeriodicScheduler expectedPeriodicScheduler(periodicScheduler);
	expectedPeriodicScheduler.setPeriodStartDate(year, month, day);

	EXPECT_THAT(actualPeriodicScheduler, Eq(expectedPeriodicScheduler));
}

