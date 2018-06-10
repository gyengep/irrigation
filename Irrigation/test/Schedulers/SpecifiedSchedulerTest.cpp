#include <gmock/gmock.h>
#include <list>
#include "Schedulers/Exceptions.h"
#include "Schedulers/SpecifiedScheduler.h"

using namespace std;

extern void checkDay(const Scheduler& scheduler, bool requestedResult, int day);



TEST(SpecifiedScheduler, init) {
	SpecifiedScheduler scheduler;

	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::MONDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::TUESDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::WEDNESDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::THURSDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::FRIDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::SATURDAY));
	EXPECT_FALSE(scheduler.isDayEnabled(SpecifiedScheduler::SUNDAY));
}

TEST(SpecifiedScheduler, enableDay) {
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

TEST(SpecifiedScheduler, enableDayInvalid) {
	SpecifiedScheduler scheduler;
	EXPECT_THROW(scheduler.enableDay((SpecifiedScheduler::Days)7, true), InvalidDayException);
}

TEST(SpecifiedScheduler, isDayEnabledInvalid) {
	SpecifiedScheduler scheduler;
	EXPECT_THROW(scheduler.isDayEnabled((SpecifiedScheduler::Days)7), InvalidDayException);
}

TEST(SpecifiedScheduler, isDayScheduled) {
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

TEST(SpecifiedScheduler, getSpecifiedSchedulerDTO) {
	SpecifiedScheduler scheduler;

	scheduler.enableDay(SpecifiedScheduler::MONDAY, true);
	scheduler.enableDay(SpecifiedScheduler::TUESDAY, true);
	scheduler.enableDay(SpecifiedScheduler::FRIDAY, true);

	const SpecifiedSchedulerDTO expectedSchedulerDTO(new list<bool>({
		false, true, true, false, false, true, false }));

	EXPECT_EQ(expectedSchedulerDTO, scheduler.getSpecifiedSchedulerDTO());
}
