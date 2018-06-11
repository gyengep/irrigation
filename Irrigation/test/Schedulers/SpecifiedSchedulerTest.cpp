#include <gmock/gmock.h>
#include <list>
#include "Schedulers/Exceptions.h"
#include "Schedulers/SpecifiedScheduler.h"

using namespace std;
using namespace testing;


extern void checkDay(const Scheduler& scheduler, bool requestedResult, int day);


TEST(SpecifiedSchedulerTest, init) {
	SpecifiedScheduler scheduler;

	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::MONDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::TUESDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::WEDNESDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::THURSDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::FRIDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::SATURDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::SUNDAY));
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
	EXPECT_THROW(scheduler.enableDay((SpecifiedScheduler::Days)7, true), InvalidDayException);
}

TEST(SpecifiedSchedulerTest, isDayEnabledInvalid) {
	SpecifiedScheduler scheduler;
	EXPECT_THROW(scheduler.isDayEnabled((SpecifiedScheduler::Days)7), InvalidDayException);
}

TEST(SpecifiedSchedulerTest, isDayScheduled) {
	SpecifiedScheduler scheduler;

	scheduler.enableDay(SpecifiedScheduler::WEDNESDAY, true);

	for (int day = 19; day < 26; day++) {
		checkDay(scheduler, 21 == day, day);
	}

	scheduler.enableDay(SpecifiedScheduler::SUNDAY, true);
	scheduler.enableDay(SpecifiedScheduler::WEDNESDAY, false);
	scheduler.enableDay(SpecifiedScheduler::THURSDAY, true);

	for (int day = 19; day < 26; day++) {
		checkDay(scheduler, 22 == day || 25 == day, day);
	}
}

TEST(SpecifiedSchedulerTest, getSpecifiedSchedulerDTO) {
	SpecifiedScheduler scheduler;

	scheduler.enableDay(SpecifiedScheduler::MONDAY, true);
	scheduler.enableDay(SpecifiedScheduler::TUESDAY, true);
	scheduler.enableDay(SpecifiedScheduler::FRIDAY, true);

	const SpecifiedSchedulerDTO expectedSchedulerDTO(new list<bool>({
		false, true, true, false, false, true, false }));

	EXPECT_EQ(expectedSchedulerDTO, scheduler.getSpecifiedSchedulerDTO());
}

TEST(SpecifiedSchedulerTest, convertSpecifiedSchedulerDTO) {
	const SpecifiedSchedulerDTO expectedSchedulerDTO(new list<bool>({
		false, true, false, true, true, false, false
	}));

	SpecifiedScheduler scheduler;
	scheduler.updateFromDTO(expectedSchedulerDTO);

	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::SUNDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::MONDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::TUESDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::WEDNESDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::THURSDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::FRIDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::SATURDAY));
	EXPECT_THAT(scheduler.getSpecifiedSchedulerDTO(), Eq(expectedSchedulerDTO));
}

TEST(SpecifiedSchedulerTest, updateLessValueFromDTO) {
	SpecifiedScheduler scheduler;
	scheduler.enableDay(SpecifiedScheduler::SUNDAY, true);
	scheduler.enableDay(SpecifiedScheduler::MONDAY, true);
	scheduler.enableDay(SpecifiedScheduler::TUESDAY, true);
	scheduler.enableDay(SpecifiedScheduler::WEDNESDAY, true);
	scheduler.enableDay(SpecifiedScheduler::THURSDAY, true);
	scheduler.enableDay(SpecifiedScheduler::FRIDAY, true);
	scheduler.enableDay(SpecifiedScheduler::SATURDAY, true);

	scheduler.updateFromDTO(SpecifiedSchedulerDTO(new list<bool>({
		false, false, false
	})));

	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::SUNDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::MONDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::TUESDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::WEDNESDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::THURSDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::FRIDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::SATURDAY));
}

TEST(SpecifiedSchedulerTest, updateMoreValueFromDTO) {
	SpecifiedScheduler scheduler;

	scheduler.updateFromDTO(SpecifiedSchedulerDTO(new list<bool>({
		true, true, true, true, true, true, true, true, true, true,
	})));

	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::SUNDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::MONDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::TUESDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::WEDNESDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::THURSDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::FRIDAY));
	EXPECT_TRUE(scheduler.isDayEnabled(SpecifiedScheduler::SATURDAY));
}
