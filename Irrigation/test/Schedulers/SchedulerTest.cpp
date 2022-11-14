#include <gmock/gmock.h>
#include "Schedulers/Scheduler.h"

using namespace std;
using namespace testing;


TEST(SchedulerTypeTest, to_string) {
	EXPECT_THAT(to_string(SchedulerType::EVERY_DAY), Eq("every-day"));
	EXPECT_THAT(to_string(SchedulerType::HOT_WEATHER), Eq("hot-weather"));
	EXPECT_THAT(to_string(SchedulerType::TEMPERATURE_DEPENDENT), Eq("temperature-dependent"));
	EXPECT_THAT(to_string(SchedulerType::WEEKLY), Eq("weekly"));
}

TEST(SchedulerTypeTest, to_string_Invalid) {
	int lastSchedulerTypeAsInt = static_cast<int>(SchedulerType::WEEKLY);
	SchedulerType schedulerType = static_cast<SchedulerType>(lastSchedulerTypeAsInt + 1);
	EXPECT_THROW(to_string(schedulerType), invalid_argument);
}

TEST(SchedulerResultTest, skipped) {
	EXPECT_FALSE(Scheduler::Result(true).isSkipped());
	EXPECT_TRUE(Scheduler::Result(false).isSkipped());

	EXPECT_TRUE(Scheduler::Result(0u).isSkipped());
	EXPECT_FALSE(Scheduler::Result(1u).isSkipped());
	EXPECT_FALSE(Scheduler::Result(2u).isSkipped());
	EXPECT_FALSE(Scheduler::Result(5u).isSkipped());
	EXPECT_FALSE(Scheduler::Result(10u).isSkipped());
	EXPECT_FALSE(Scheduler::Result(100u).isSkipped());
}

TEST(SchedulerResultTest, adjustment) {
	EXPECT_THAT(Scheduler::Result(true).getAdjustment(), Eq(100));
	EXPECT_THAT(Scheduler::Result(false).getAdjustment(), Eq(0));

	EXPECT_THAT(Scheduler::Result(0u).getAdjustment(), Eq(0));
	EXPECT_THAT(Scheduler::Result(1u).getAdjustment(), Eq(1));
	EXPECT_THAT(Scheduler::Result(2u).getAdjustment(), Eq(2));
	EXPECT_THAT(Scheduler::Result(5u).getAdjustment(), Eq(5));
	EXPECT_THAT(Scheduler::Result(10u).getAdjustment(), Eq(10));
	EXPECT_THAT(Scheduler::Result(100u).getAdjustment(), Eq(100));
}
