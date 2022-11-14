#include "Schedulers/WeeklySchedulerImpl.h"
#include "Exceptions/Exceptions.h"
#include "Dto2Object/WeeklySchedulerSamples.h"
#include <gmock/gmock.h>
#include <list>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void checkDay(int year, int month, int day, Scheduler& scheduler, bool expectedResult) {
	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {
				const LocalDateTime localDateTime(year, month, day, hour, min, sec);
				ASSERT_THAT(scheduler.process(localDateTime), Pointee(Scheduler::Result(expectedResult)));
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

	const int year = 2018;
	const int month = 11;

	for (int day = 19; day < 26; day++) {
		checkDay(year, month, day, scheduler, 21 == day);
	}

	scheduler.enableDay(WeeklySchedulerImpl::WEDNESDAY, false);
	scheduler.enableDay(WeeklySchedulerImpl::THURSDAY, true);
	scheduler.enableDay(WeeklySchedulerImpl::SUNDAY, true);

	for (int day = 19; day < 26; day++) {
		checkDay(year, month, day, scheduler, 22 == day || 25 == day);
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(WeeklySchedulerImplTest, toWeeklySchedulerDto) {
	EXPECT_THAT(
		Dto2ObjectTest::WeeklySchedulerSample1().getObjectPtr()->toWeeklySchedulerDto(),
		Eq(Dto2ObjectTest::WeeklySchedulerSample1().getDto())
	);

	EXPECT_THAT(
		Dto2ObjectTest::WeeklySchedulerSample2().getObjectPtr()->toWeeklySchedulerDto(),
		Eq(Dto2ObjectTest::WeeklySchedulerSample2().getDto())
	);

	EXPECT_THAT(
		Dto2ObjectTest::WeeklySchedulerSample3().getObjectPtr()->toWeeklySchedulerDto(),
		Eq(Dto2ObjectTest::WeeklySchedulerSample3().getDto())
	);

	EXPECT_THAT(
		Dto2ObjectTest::WeeklySchedulerSample4().getObjectPtr()->toWeeklySchedulerDto(),
		Eq(Dto2ObjectTest::WeeklySchedulerSample4().getDto())
	);
}

///////////////////////////////////////////////////////////////////////////////

TEST(WeeklySchedulerImplTest, updateFromDto_empty) {
	WeeklySchedulerImpl scheduler({ true, true, true, false, false, false, false });

	scheduler.updateFromWeeklySchedulerDto(WeeklySchedulerDTO());

	EXPECT_TRUE(scheduler.isDayEnabled(WeeklySchedulerImpl::MONDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklySchedulerImpl::TUESDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklySchedulerImpl::WEDNESDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::THURSDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::FRIDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::SATURDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::SUNDAY));
}

TEST(WeeklySchedulerImplTest, updateFromDto_partial_all) {
	WeeklySchedulerImpl scheduler({ true, true, true, false, false, false, false });

	scheduler.updateFromWeeklySchedulerDto(WeeklySchedulerDTO(std::list<bool> { false, false, true, true, false, true, false }));

	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::MONDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::TUESDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklySchedulerImpl::WEDNESDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklySchedulerImpl::THURSDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::FRIDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklySchedulerImpl::SATURDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::SUNDAY));

	scheduler.updateFromWeeklySchedulerDto(WeeklySchedulerDTO(std::list<bool> { true, true, true, true, false, false, false }));

	EXPECT_TRUE(scheduler.isDayEnabled(WeeklySchedulerImpl::MONDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklySchedulerImpl::TUESDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklySchedulerImpl::WEDNESDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklySchedulerImpl::THURSDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::FRIDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::SATURDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklySchedulerImpl::SUNDAY));
}

