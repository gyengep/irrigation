#include <gmock/gmock.h>
#include <list>
#include "Exceptions/Exceptions.h"
#include "Schedulers/SpecifiedScheduler.h"

using namespace std;
using namespace testing;


extern void checkDay(const Scheduler& scheduler, bool requestedResult, int day);


TEST(SpecifiedSchedulerTest, init) {
	SpecifiedScheduler scheduler;

	EXPECT_EQ(100, scheduler.getAdjustment());
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::MONDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::TUESDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::WEDNESDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::THURSDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::FRIDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::SATURDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::SUNDAY));
}

TEST(SpecifiedSchedulerTest, setAdjustment) {
	SpecifiedScheduler scheduler;

	scheduler.setAdjustment(53);
	EXPECT_EQ(53, scheduler.getAdjustment());
}

TEST(SpecifiedSchedulerTest, enableDay) {
	SpecifiedScheduler scheduler;

	scheduler.enableDay(SpecifiedScheduler::WEDNESDAY, true);

	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::MONDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::TUESDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::WEDNESDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::THURSDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::FRIDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::SATURDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::SUNDAY));

	scheduler.enableDay(SpecifiedScheduler::FRIDAY, true);

	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::MONDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::TUESDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::WEDNESDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::THURSDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::FRIDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::SATURDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::SUNDAY));

	scheduler.enableDay(SpecifiedScheduler::WEDNESDAY, false);

	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::MONDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::TUESDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::WEDNESDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::THURSDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::FRIDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::SATURDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::SUNDAY));
}

TEST(SpecifiedSchedulerTest, enableDayInvalid) {
	SpecifiedScheduler scheduler;
	EXPECT_THROW(scheduler.enableDay(7, true), IndexOutOfBoundsException);
}

TEST(SpecifiedSchedulerTest, isDayEnabledInvalid) {
	SpecifiedScheduler scheduler;
	EXPECT_THROW(scheduler.isDayEnabled(7), IndexOutOfBoundsException);
}

TEST(SpecifiedSchedulerTest, isDayScheduled) {
	SpecifiedScheduler scheduler;

	scheduler.enableDay(SpecifiedScheduler::WEDNESDAY, true);

	for (int day = 5; day < 12; day++) {
		checkDay(scheduler, 7 == day, day);
	}

	scheduler.enableDay(SpecifiedScheduler::WEDNESDAY, false);
	scheduler.enableDay(SpecifiedScheduler::THURSDAY, true);
	scheduler.enableDay(SpecifiedScheduler::SUNDAY, true);

	for (int day = 5; day < 12; day++) {
		checkDay(scheduler, 8 == day || 11 == day, day);
	}
}

TEST(SpecifiedSchedulerTest, getSpecifiedSchedulerDTO) {
	SpecifiedScheduler scheduler;

	scheduler.setAdjustment(123);
	scheduler.enableDay(SpecifiedScheduler::MONDAY, true);
	scheduler.enableDay(SpecifiedScheduler::TUESDAY, true);
	scheduler.enableDay(SpecifiedScheduler::FRIDAY, true);

	const SpecifiedSchedulerDTO expectedSchedulerDTO(
			123,
			list<bool>({true, true, false, false, true, false, false })
			);

	EXPECT_EQ(expectedSchedulerDTO, scheduler.getSpecifiedSchedulerDTO());
}

TEST(SpecifiedSchedulerTest, convertSpecifiedSchedulerDTO) {
	const SpecifiedSchedulerDTO expectedSchedulerDTO(
		120,
		list<bool>({true, false, true, true, false, false, false})
		);

	SpecifiedScheduler scheduler;
	scheduler.updateFromDTO(expectedSchedulerDTO);

	EXPECT_EQ(120, scheduler.getAdjustment());
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::MONDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::TUESDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::WEDNESDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::THURSDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::FRIDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::SATURDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::SUNDAY));
	EXPECT_THAT(scheduler.getSpecifiedSchedulerDTO(), Eq(expectedSchedulerDTO));
}

TEST(SpecifiedSchedulerTest, updateLessValueFromDTO) {
	SpecifiedScheduler scheduler;
	scheduler.enableDay(SpecifiedScheduler::MONDAY, true);
	scheduler.enableDay(SpecifiedScheduler::TUESDAY, true);
	scheduler.enableDay(SpecifiedScheduler::WEDNESDAY, true);
	scheduler.enableDay(SpecifiedScheduler::THURSDAY, true);
	scheduler.enableDay(SpecifiedScheduler::FRIDAY, true);
	scheduler.enableDay(SpecifiedScheduler::SATURDAY, true);
	scheduler.enableDay(SpecifiedScheduler::SUNDAY, true);

	scheduler.updateFromDTO(SpecifiedSchedulerDTO(
			100,
			list<bool>({false, false, false})
			));

	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::MONDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::TUESDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::WEDNESDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::THURSDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::FRIDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::SATURDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::SUNDAY));
}

TEST(SpecifiedSchedulerTest, updateMoreValueFromDTO) {
	SpecifiedScheduler scheduler;

	scheduler.updateFromDTO(SpecifiedSchedulerDTO(
		100,
		list<bool>({true, true, true, true, true, true, true, true, true, true})
		));

	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::MONDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::TUESDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::WEDNESDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::THURSDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::FRIDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::SATURDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::SUNDAY));
}
