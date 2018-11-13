#include <gmock/gmock.h>
#include <list>
#include "Exceptions/Exceptions.h"
#include "Schedulers/WeeklyScheduler.h"

using namespace std;
using namespace testing;


extern void checkDay(const Scheduler& scheduler, bool requestedResult, int day);


TEST(WeeklySchedulerTest, init) {
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

	for (int day = 5; day < 12; day++) {
		checkDay(scheduler, 7 == day, day);
	}

	scheduler.enableDay(WeeklyScheduler::WEDNESDAY, false);
	scheduler.enableDay(WeeklyScheduler::THURSDAY, true);
	scheduler.enableDay(WeeklyScheduler::SUNDAY, true);

	for (int day = 5; day < 12; day++) {
		checkDay(scheduler, 8 == day || 11 == day, day);
	}
}

TEST(WeeklySchedulerTest, getWeeklySchedulerDTO) {
	WeeklyScheduler scheduler;

	scheduler.setAdjustment(123);
	scheduler.enableDay(WeeklyScheduler::MONDAY, true);
	scheduler.enableDay(WeeklyScheduler::TUESDAY, true);
	scheduler.enableDay(WeeklyScheduler::FRIDAY, true);

	const WeeklySchedulerDTO expectedSchedulerDTO(
			123,
			list<bool>({true, true, false, false, true, false, false })
			);

	EXPECT_EQ(expectedSchedulerDTO, scheduler.getWeeklySchedulerDTO());
}

TEST(WeeklySchedulerTest, convertWeeklySchedulerDTO) {
	const WeeklySchedulerDTO expectedSchedulerDTO(
		120,
		list<bool>({true, false, true, true, false, false, false})
		);

	WeeklyScheduler scheduler;
	scheduler.updateFromDTO(expectedSchedulerDTO);

	EXPECT_EQ(120, scheduler.getAdjustment());
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklyScheduler::MONDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::TUESDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklyScheduler::WEDNESDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklyScheduler::THURSDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::FRIDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::SATURDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::SUNDAY));
	EXPECT_THAT(scheduler.getWeeklySchedulerDTO(), Eq(expectedSchedulerDTO));
}

TEST(WeeklySchedulerTest, updateLessValueFromDTO) {
	WeeklyScheduler scheduler;
	scheduler.enableDay(WeeklyScheduler::MONDAY, true);
	scheduler.enableDay(WeeklyScheduler::TUESDAY, true);
	scheduler.enableDay(WeeklyScheduler::WEDNESDAY, true);
	scheduler.enableDay(WeeklyScheduler::THURSDAY, true);
	scheduler.enableDay(WeeklyScheduler::FRIDAY, true);
	scheduler.enableDay(WeeklyScheduler::SATURDAY, true);
	scheduler.enableDay(WeeklyScheduler::SUNDAY, true);

	scheduler.updateFromDTO(WeeklySchedulerDTO(
			100,
			list<bool>({false, false, false})
			));

	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::MONDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::TUESDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(WeeklyScheduler::WEDNESDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklyScheduler::THURSDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklyScheduler::FRIDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklyScheduler::SATURDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklyScheduler::SUNDAY));
}

TEST(WeeklySchedulerTest, updateMoreValueFromDTO) {
	WeeklyScheduler scheduler;

	scheduler.updateFromDTO(WeeklySchedulerDTO(
		100,
		list<bool>({true, true, true, true, true, true, true, true, true, true})
		));

	EXPECT_TRUE(scheduler.isDayEnabled(WeeklyScheduler::MONDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklyScheduler::TUESDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklyScheduler::WEDNESDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklyScheduler::THURSDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklyScheduler::FRIDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklyScheduler::SATURDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(WeeklyScheduler::SUNDAY));
}