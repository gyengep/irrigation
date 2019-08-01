#include <gmock/gmock.h>
#include "Exceptions/Exceptions.h"
#include "Schedulers/PeriodicScheduler.h"
#include "Utils/TimeConversion.h"
#include "Dto2Object/PeriodicSchedulerSamples.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;

extern bool isScheduled(Scheduler& scheduler, const tm& timeinfo);

TEST(PeriodicSchedulerTest, defaultConstructor) {
	PeriodicScheduler scheduler;

	EXPECT_THAT(scheduler.getPeriod(), 0);
	EXPECT_THAT(scheduler.getPeriodStartYear(), 1970);
	EXPECT_THAT(scheduler.getPeriodStartMonth(), 1);
	EXPECT_THAT(scheduler.getPeriodStartDay(), 1);
}

TEST(PeriodicSchedulerTest, parametrizedConstructor) {
	PeriodicScheduler scheduler(vector<bool>({true, false, true}), 2011, 12, 13);

	EXPECT_THAT(scheduler.getPeriod(), Eq(3));
	EXPECT_TRUE(scheduler.isDayEnabled(0));
	EXPECT_FALSE(scheduler.isDayEnabled(1));
	EXPECT_TRUE(scheduler.isDayEnabled(2));
	EXPECT_THAT(scheduler.getPeriodStartYear(), Eq(2011));
	EXPECT_THAT(scheduler.getPeriodStartMonth(), Eq(12));
	EXPECT_THAT(scheduler.getPeriodStartDay(), Eq(13));
}

TEST(PeriodicSchedulerTest, copyConstructor) {
	PeriodicScheduler scheduler(PeriodicScheduler(vector<bool>({true, false, true}), 2011, 12, 13));

	EXPECT_THAT(scheduler.getPeriod(), Eq(3));
	EXPECT_TRUE(scheduler.isDayEnabled(0));
	EXPECT_FALSE(scheduler.isDayEnabled(1));
	EXPECT_TRUE(scheduler.isDayEnabled(2));
	EXPECT_THAT(scheduler.getPeriodStartYear(), Eq(2011));
	EXPECT_THAT(scheduler.getPeriodStartMonth(), Eq(12));
	EXPECT_THAT(scheduler.getPeriodStartDay(), Eq(13));
}

TEST(PeriodicSchedulerTest, equalsOperator) {
	PeriodicScheduler scheduler1;
	PeriodicScheduler scheduler2;

	EXPECT_TRUE(scheduler1 == scheduler2);
	EXPECT_TRUE(scheduler2 == scheduler1);

	{
		const unsigned period1 = 2;
		const unsigned period2 = 3;

		scheduler1.setPeriod(period1);
		EXPECT_FALSE(scheduler1 == scheduler2);
		EXPECT_FALSE(scheduler2 == scheduler1);

		scheduler2.setPeriod(period2);
		EXPECT_FALSE(scheduler1 == scheduler2);
		EXPECT_FALSE(scheduler2 == scheduler1);

		scheduler1.setPeriod(period2);
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

	{
		const unsigned year1 = 2010;
		const unsigned year2 = 2020;
		const unsigned month = 10;
		const unsigned day = 20;

		scheduler1.setPeriodStartDate(year1, month, day);
		EXPECT_FALSE(scheduler1 == scheduler2);
		EXPECT_FALSE(scheduler2 == scheduler1);

		scheduler2.setPeriodStartDate(year2, month, day);
		EXPECT_FALSE(scheduler1 == scheduler2);
		EXPECT_FALSE(scheduler2 == scheduler1);

		scheduler1.setPeriodStartDate(year2, month, day);
		EXPECT_TRUE(scheduler1 == scheduler2);
		EXPECT_TRUE(scheduler2 == scheduler1);
	}

	{
		const unsigned year = 2010;
		const unsigned month1 = 5;
		const unsigned month2 = 6;
		const unsigned day = 20;

		scheduler1.setPeriodStartDate(year, month1, day);
		EXPECT_FALSE(scheduler1 == scheduler2);
		EXPECT_FALSE(scheduler2 == scheduler1);

		scheduler2.setPeriodStartDate(year, month2, day);
		EXPECT_FALSE(scheduler1 == scheduler2);
		EXPECT_FALSE(scheduler2 == scheduler1);

		scheduler1.setPeriodStartDate(year, month2, day);
		EXPECT_TRUE(scheduler1 == scheduler2);
		EXPECT_TRUE(scheduler2 == scheduler1);
	}

	{
		const unsigned year = 2010;
		const unsigned month = 1;
		const unsigned day1 = 16;
		const unsigned day2 = 26;

		scheduler1.setPeriodStartDate(year, month, day1);
		EXPECT_FALSE(scheduler1 == scheduler2);
		EXPECT_FALSE(scheduler2 == scheduler1);

		scheduler2.setPeriodStartDate(year, month, day2);
		EXPECT_FALSE(scheduler1 == scheduler2);
		EXPECT_FALSE(scheduler2 == scheduler1);

		scheduler1.setPeriodStartDate(year, month, day2);
		EXPECT_TRUE(scheduler1 == scheduler2);
		EXPECT_TRUE(scheduler2 == scheduler1);
	}
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

	EXPECT_TRUE(isScheduled(scheduler, toCalendarTime(2018, 11, 5)));
	EXPECT_FALSE(isScheduled(scheduler, toCalendarTime(2018, 11, 6)));
	EXPECT_TRUE(isScheduled(scheduler, toCalendarTime(2018, 11, 7)));
	EXPECT_TRUE(isScheduled(scheduler, toCalendarTime(2018, 11, 8)));
	EXPECT_FALSE(isScheduled(scheduler, toCalendarTime(2018, 11, 9)));
	EXPECT_FALSE(isScheduled(scheduler, toCalendarTime(2018, 11, 10)));
	EXPECT_FALSE(isScheduled(scheduler, toCalendarTime(2018, 11, 11)));

	EXPECT_TRUE(isScheduled(scheduler, toCalendarTime(2018, 11, 12)));
	EXPECT_FALSE(isScheduled(scheduler, toCalendarTime(2018, 11, 13)));
	EXPECT_TRUE(isScheduled(scheduler, toCalendarTime(2018, 11, 14)));
	EXPECT_TRUE(isScheduled(scheduler, toCalendarTime(2018, 11, 15)));
	EXPECT_FALSE(isScheduled(scheduler, toCalendarTime(2018, 11, 16)));
	EXPECT_FALSE(isScheduled(scheduler, toCalendarTime(2018, 11, 17)));
	EXPECT_FALSE(isScheduled(scheduler, toCalendarTime(2018, 11, 18)));

	EXPECT_TRUE(isScheduled(scheduler, toCalendarTime(2018, 11, 19)));
	EXPECT_FALSE(isScheduled(scheduler, toCalendarTime(2018, 11, 20)));
	EXPECT_TRUE(isScheduled(scheduler, toCalendarTime(2018, 11, 21)));
	EXPECT_TRUE(isScheduled(scheduler, toCalendarTime(2018, 11, 22)));
	EXPECT_FALSE(isScheduled(scheduler, toCalendarTime(2018, 11, 23)));
	EXPECT_FALSE(isScheduled(scheduler, toCalendarTime(2018, 11, 24)));
	EXPECT_FALSE(isScheduled(scheduler, toCalendarTime(2018, 11, 25)));

	///////////////////////////////////////////////////////////////////////////

	EXPECT_TRUE(isScheduled(scheduler, toCalendarTime(2017, 11, 13)));
	EXPECT_FALSE(isScheduled(scheduler, toCalendarTime(2017, 11, 14)));
	EXPECT_TRUE(isScheduled(scheduler, toCalendarTime(2017, 11, 15)));
	EXPECT_TRUE(isScheduled(scheduler, toCalendarTime(2017, 11, 16)));
	EXPECT_FALSE(isScheduled(scheduler, toCalendarTime(2017, 11, 17)));
	EXPECT_FALSE(isScheduled(scheduler, toCalendarTime(2017, 11, 18)));
	EXPECT_FALSE(isScheduled(scheduler, toCalendarTime(2017, 11, 19)));

	///////////////////////////////////////////////////////////////////////////

	EXPECT_TRUE(isScheduled(scheduler, toCalendarTime(2016, 11, 14)));
	EXPECT_FALSE(isScheduled(scheduler, toCalendarTime(2016, 11, 15)));
	EXPECT_TRUE(isScheduled(scheduler, toCalendarTime(2016, 11, 16)));
	EXPECT_TRUE(isScheduled(scheduler, toCalendarTime(2016, 11, 17)));
	EXPECT_FALSE(isScheduled(scheduler, toCalendarTime(2016, 11, 18)));
	EXPECT_FALSE(isScheduled(scheduler, toCalendarTime(2016, 11, 19)));
	EXPECT_FALSE(isScheduled(scheduler, toCalendarTime(2016, 11, 20)));
}

TEST(PeriodicSchedulerTest, isDayScheduledInvalid) {
	PeriodicScheduler scheduler;

	scheduler.setPeriod(0);
	EXPECT_FALSE(isScheduled(scheduler, toCalendarTime(2018, 11, 5)));
}

///////////////////////////////////////////////////////////////////////////////

void testToPeriodicSchedulerDto(const PeriodicSchedulerSample& periodicSchedulerSample) {
	const shared_ptr<PeriodicScheduler> periodicScheduler = periodicSchedulerSample.getObject();
	const PeriodicSchedulerDTO& expectedPeriodicSchedulerDto = periodicSchedulerSample.getDto();
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
	EXPECT_THAT(periodicScheduler, Not(Pointee(*periodicSchedulerSample.getObject())));
	periodicScheduler->updateFromPeriodicSchedulerDto(periodicSchedulerSample.getDto());
	EXPECT_THAT(periodicScheduler, Pointee(*periodicSchedulerSample.getObject()));
}

TEST(PeriodicSchedulerTest, updateFromPeriodicSchedulerDto1) {
	shared_ptr<PeriodicScheduler> periodicScheduler = shared_ptr<PeriodicScheduler>(new PeriodicScheduler());
	testUpdateFromPeriodicSchedulerDto(periodicScheduler, PeriodicSchedulerSample1());
}

TEST(PeriodicSchedulerTest, updateFromPeriodicSchedulerDto2) {
	shared_ptr<PeriodicScheduler> periodicScheduler = PeriodicSchedulerSample1().getObject();
	testUpdateFromPeriodicSchedulerDto(periodicScheduler, PeriodicSchedulerSample2());
}

TEST(PeriodicSchedulerTest, updateFromPeriodicSchedulerDto3) {
	shared_ptr<PeriodicScheduler> periodicScheduler = PeriodicSchedulerSample2().getObject();
	testUpdateFromPeriodicSchedulerDto(periodicScheduler, PeriodicSchedulerSample3());
}

TEST(PeriodicSchedulerTest, updateFromPeriodicSchedulerDto4) {
	shared_ptr<PeriodicScheduler> periodicScheduler = PeriodicSchedulerSample3().getObject();
	testUpdateFromPeriodicSchedulerDto(periodicScheduler, PeriodicSchedulerSample4());
}

///////////////////////////////////////////////////////////////////////////////

TEST(PeriodicSchedulerTest, partialUpdateFromPeriodicSchedulerDto_empty) {
	PeriodicScheduler actualPeriodicScheduler(*PeriodicSchedulerSample1().getObject());
	PeriodicScheduler expectedPeriodicScheduler(*PeriodicSchedulerSample1().getObject());

	actualPeriodicScheduler.updateFromPeriodicSchedulerDto(PeriodicSchedulerDTO());

	EXPECT_THAT(actualPeriodicScheduler, Eq(expectedPeriodicScheduler));
}

TEST(PeriodicSchedulerTest, partialUpdateFromPeriodicSchedulerDto_days) {
	const list<bool> days1({ true, false });
	const list<bool> days2({ false, true, false, true });

	PeriodicScheduler actualPeriodicScheduler(*PeriodicSchedulerSample3().getObject());
	PeriodicScheduler expectedPeriodicScheduler(*PeriodicSchedulerSample3().getObject());

	actualPeriodicScheduler.updateFromPeriodicSchedulerDto(PeriodicSchedulerDTO().setValues(list<bool>(days1)));
	expectedPeriodicScheduler.setPeriod(days1.size());
	for (size_t i = 0; i < days1.size(); ++i) {
		expectedPeriodicScheduler.enableDay(i, *next(days1.begin(), i));
	}
	EXPECT_THAT(actualPeriodicScheduler, Eq(expectedPeriodicScheduler));

	actualPeriodicScheduler.updateFromPeriodicSchedulerDto(PeriodicSchedulerDTO().setValues(list<bool>(days2)));
	expectedPeriodicScheduler.setPeriod(days2.size());
	for (size_t i = 0; i < days2.size(); ++i) {
		expectedPeriodicScheduler.enableDay(i, *next(days2.begin(), i));
	}
	EXPECT_THAT(actualPeriodicScheduler, Eq(expectedPeriodicScheduler));
}

TEST(PeriodicSchedulerTest, partialUpdateFromPeriodicSchedulerDto_periodStart) {
	const unsigned year1 = 1999;
	const unsigned month1 = 11;
	const unsigned day1 = 25;
	const unsigned year2 = 2003;
	const unsigned month2 = 9;
	const unsigned day2 = 11;

	PeriodicScheduler actualPeriodicScheduler(*PeriodicSchedulerSample4().getObject());
	PeriodicScheduler expectedPeriodicScheduler(*PeriodicSchedulerSample4().getObject());

	actualPeriodicScheduler.updateFromPeriodicSchedulerDto(
			PeriodicSchedulerDTO().
			setPeriodStartYear(year1).
			setPeriodStartMonth(month1).
			setPeriodStartDay(day1));
	expectedPeriodicScheduler.setPeriodStartDate(year1, month1, day1);
	EXPECT_THAT(actualPeriodicScheduler, Eq(expectedPeriodicScheduler));

	actualPeriodicScheduler.updateFromPeriodicSchedulerDto(
			PeriodicSchedulerDTO().
			setPeriodStartYear(year2).
			setPeriodStartMonth(month2).
			setPeriodStartDay(day2));
	expectedPeriodicScheduler.setPeriodStartDate(year2, month2, day2);
	EXPECT_THAT(actualPeriodicScheduler, Eq(expectedPeriodicScheduler));
}

