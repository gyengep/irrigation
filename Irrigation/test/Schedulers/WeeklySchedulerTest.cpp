#include <gmock/gmock.h>
#include <list>
#include "Exceptions/Exceptions.h"
#include "Schedulers/WeeklyScheduler.h"
#include "Utils/TimeConversion.h"
#include "WeeklySchedulerSamples.h"

using namespace std;
using namespace testing;
using namespace LogicTest;

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

	EXPECT_EQ(100, scheduler.getAdjustment());
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::MONDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::TUESDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::WEDNESDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::THURSDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::FRIDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::SATURDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::SUNDAY));
}

TEST(WeeklySchedulerTest, setAdjustment) {
	WeeklyScheduler scheduler;

	scheduler.setAdjustment(53);
	EXPECT_EQ(53, scheduler.getAdjustment());
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
	const shared_ptr<WeeklyScheduler> weeklyScheduler = weeklySchedulerSample.getWeeklyScheduler();
	const WeeklySchedulerDTO& expectedWeeklySchedulerDto = weeklySchedulerSample.getWeeklySchedulerDto();
	EXPECT_THAT(weeklyScheduler->toWeeklySchedulerDto(), Eq(expectedWeeklySchedulerDto));
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
	EXPECT_THAT(weeklyScheduler, Not(Pointee(*weeklySchedulerSample.getWeeklyScheduler())));
	weeklyScheduler->updateFromWeeklySchedulerDto(weeklySchedulerSample.getWeeklySchedulerDto());
	EXPECT_THAT(weeklyScheduler, Pointee(*weeklySchedulerSample.getWeeklyScheduler()));
}

TEST(WeeklySchedulerTest, updateFromWeeklySchedulerDto1) {
	shared_ptr<WeeklyScheduler> weeklyScheduler = shared_ptr<WeeklyScheduler>(new WeeklyScheduler());
	testUpdateFromWeeklySchedulerDto(weeklyScheduler, WeeklySchedulerSample1());
}

TEST(WeeklySchedulerTest, updateFromWeeklySchedulerDto2) {
	shared_ptr<WeeklyScheduler> weeklyScheduler = WeeklySchedulerSample1().getWeeklyScheduler();
	testUpdateFromWeeklySchedulerDto(weeklyScheduler, WeeklySchedulerSample2());
}

TEST(WeeklySchedulerTest, updateFromWeeklySchedulerDto3) {
	shared_ptr<WeeklyScheduler> weeklyScheduler = WeeklySchedulerSample2().getWeeklyScheduler();
	testUpdateFromWeeklySchedulerDto(weeklyScheduler, WeeklySchedulerSample3());
}

TEST(WeeklySchedulerTest, updateFromWeeklySchedulerDto4) {
	shared_ptr<WeeklyScheduler> weeklyScheduler = WeeklySchedulerSample3().getWeeklyScheduler();
	testUpdateFromWeeklySchedulerDto(weeklyScheduler, WeeklySchedulerSample4());
}

///////////////////////////////////////////////////////////////////////////////

TEST(WeeklySchedulerTest, partialUpdateFromWeeklySchedulerDto_empty) {
	const WeeklyScheduler weeklyScheduler(*WeeklySchedulerSample1().getWeeklyScheduler());

	WeeklyScheduler actualWeeklyScheduler(weeklyScheduler);
	actualWeeklyScheduler.updateFromWeeklySchedulerDto(WeeklySchedulerDTO());

	WeeklyScheduler expectedWeeklyScheduler(weeklyScheduler);

	EXPECT_THAT(actualWeeklyScheduler, Eq(expectedWeeklyScheduler));
}

TEST(WeeklySchedulerTest, partialUpdateFromWeeklySchedulerDto_adjustment) {
	const WeeklyScheduler weeklyScheduler(*WeeklySchedulerSample2().getWeeklyScheduler());
	const unsigned adjustment = 80;

	WeeklyScheduler actualWeeklyScheduler(weeklyScheduler);
	actualWeeklyScheduler.updateFromWeeklySchedulerDto(WeeklySchedulerDTO().setAdjustment(adjustment));

	WeeklyScheduler expectedWeeklyScheduler(weeklyScheduler);
	expectedWeeklyScheduler.setAdjustment(adjustment);


	EXPECT_THAT(actualWeeklyScheduler, Eq(expectedWeeklyScheduler));
}

TEST(WeeklySchedulerTest, partialUpdateFromWeeklySchedulerDto_days) {
	const WeeklyScheduler weeklyScheduler(*WeeklySchedulerSample3().getWeeklyScheduler());
	const list<bool> days({ false, false, true, true, false, true, false});

	WeeklyScheduler actualWeeklyScheduler(weeklyScheduler);
	actualWeeklyScheduler.updateFromWeeklySchedulerDto(WeeklySchedulerDTO().setValues(list<bool>(days)));

	WeeklyScheduler expectedWeeklyScheduler(weeklyScheduler);
	for (size_t i = 0; i < days.size(); ++i) {
		expectedWeeklyScheduler.enableDay(i, *next(days.begin(), i));
	}

	EXPECT_THAT(actualWeeklyScheduler, Eq(expectedWeeklyScheduler));
}

