#include "WeeklySchedulerImplTest.h"
#include "Exceptions/Exceptions.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(WeeklySchedulerImplConstructorTest, defaultConstructor) {
	WeeklySchedulerImpl weeklyScheduler;

	EXPECT_FALSE(weeklyScheduler.isDayEnabled(WeeklySchedulerImpl::MONDAY));
	EXPECT_FALSE(weeklyScheduler.isDayEnabled(WeeklySchedulerImpl::TUESDAY));
	EXPECT_FALSE(weeklyScheduler.isDayEnabled(WeeklySchedulerImpl::WEDNESDAY));
	EXPECT_FALSE(weeklyScheduler.isDayEnabled(WeeklySchedulerImpl::THURSDAY));
	EXPECT_FALSE(weeklyScheduler.isDayEnabled(WeeklySchedulerImpl::FRIDAY));
	EXPECT_FALSE(weeklyScheduler.isDayEnabled(WeeklySchedulerImpl::SATURDAY));
	EXPECT_FALSE(weeklyScheduler.isDayEnabled(WeeklySchedulerImpl::SUNDAY));
}

TEST(WeeklySchedulerImplConstructorTest, initializerConstructor) {
	WeeklySchedulerImpl weeklyScheduler({true, false, true, true, false, false, false});

	EXPECT_TRUE(weeklyScheduler.isDayEnabled(0));
	EXPECT_FALSE(weeklyScheduler.isDayEnabled(1));
	EXPECT_TRUE(weeklyScheduler.isDayEnabled(2));
	EXPECT_TRUE(weeklyScheduler.isDayEnabled(3));
	EXPECT_FALSE(weeklyScheduler.isDayEnabled(4));
	EXPECT_FALSE(weeklyScheduler.isDayEnabled(5));
	EXPECT_FALSE(weeklyScheduler.isDayEnabled(6));
}

///////////////////////////////////////////////////////////////////////////////

void WeeklySchedulerImplTest::SetUp() {
	weeklyScheduler = std::make_shared<WeeklySchedulerImpl>();
}

void WeeklySchedulerImplTest::TearDown() {

}

void WeeklySchedulerImplTest::checkDay(int year, int month, int day, bool expectedResult) {
	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {
				const LocalDateTime localDateTime(year, month, day, hour, min, sec);
				ASSERT_THAT(weeklyScheduler->process(localDateTime), Pointee(Scheduler::Result(expectedResult)));
			}
		}
	}
}

TEST_F(WeeklySchedulerImplTest, enableDay) {
	weeklyScheduler->enableDay(WeeklySchedulerImpl::WEDNESDAY, true);

	EXPECT_FALSE(weeklyScheduler->isDayEnabled(WeeklySchedulerImpl::MONDAY));
	EXPECT_FALSE(weeklyScheduler->isDayEnabled(WeeklySchedulerImpl::TUESDAY));
	EXPECT_TRUE(weeklyScheduler->isDayEnabled(WeeklySchedulerImpl::WEDNESDAY));
	EXPECT_FALSE(weeklyScheduler->isDayEnabled(WeeklySchedulerImpl::THURSDAY));
	EXPECT_FALSE(weeklyScheduler->isDayEnabled(WeeklySchedulerImpl::FRIDAY));
	EXPECT_FALSE(weeklyScheduler->isDayEnabled(WeeklySchedulerImpl::SATURDAY));
	EXPECT_FALSE(weeklyScheduler->isDayEnabled(WeeklySchedulerImpl::SUNDAY));

	weeklyScheduler->enableDay(WeeklySchedulerImpl::FRIDAY, true);

	EXPECT_FALSE(weeklyScheduler->isDayEnabled(WeeklySchedulerImpl::MONDAY));
	EXPECT_FALSE(weeklyScheduler->isDayEnabled(WeeklySchedulerImpl::TUESDAY));
	EXPECT_TRUE(weeklyScheduler->isDayEnabled(WeeklySchedulerImpl::WEDNESDAY));
	EXPECT_FALSE(weeklyScheduler->isDayEnabled(WeeklySchedulerImpl::THURSDAY));
	EXPECT_TRUE(weeklyScheduler->isDayEnabled(WeeklySchedulerImpl::FRIDAY));
	EXPECT_FALSE(weeklyScheduler->isDayEnabled(WeeklySchedulerImpl::SATURDAY));
	EXPECT_FALSE(weeklyScheduler->isDayEnabled(WeeklySchedulerImpl::SUNDAY));

	weeklyScheduler->enableDay(WeeklySchedulerImpl::WEDNESDAY, false);

	EXPECT_FALSE(weeklyScheduler->isDayEnabled(WeeklySchedulerImpl::MONDAY));
	EXPECT_FALSE(weeklyScheduler->isDayEnabled(WeeklySchedulerImpl::TUESDAY));
	EXPECT_FALSE(weeklyScheduler->isDayEnabled(WeeklySchedulerImpl::WEDNESDAY));
	EXPECT_FALSE(weeklyScheduler->isDayEnabled(WeeklySchedulerImpl::THURSDAY));
	EXPECT_TRUE(weeklyScheduler->isDayEnabled(WeeklySchedulerImpl::FRIDAY));
	EXPECT_FALSE(weeklyScheduler->isDayEnabled(WeeklySchedulerImpl::SATURDAY));
	EXPECT_FALSE(weeklyScheduler->isDayEnabled(WeeklySchedulerImpl::SUNDAY));
}

TEST_F(WeeklySchedulerImplTest, enableDayInvalid) {
	EXPECT_THROW(weeklyScheduler->enableDay(7, true), IndexOutOfBoundsException);
}

TEST_F(WeeklySchedulerImplTest, isDayEnabledInvalid) {
	EXPECT_THROW(weeklyScheduler->isDayEnabled(7), IndexOutOfBoundsException);
}

TEST_F(WeeklySchedulerImplTest, isDayScheduled) {
	weeklyScheduler->enableDay(WeeklySchedulerImpl::WEDNESDAY, true);

	const int year = 2018;
	const int month = 11;

	for (int day = 19; day < 26; day++) {
		checkDay(year, month, day, 21 == day);
	}

	weeklyScheduler->enableDay(WeeklySchedulerImpl::WEDNESDAY, false);
	weeklyScheduler->enableDay(WeeklySchedulerImpl::THURSDAY, true);
	weeklyScheduler->enableDay(WeeklySchedulerImpl::SUNDAY, true);

	for (int day = 19; day < 26; day++) {
		checkDay(year, month, day, 22 == day || 25 == day);
	}
}
