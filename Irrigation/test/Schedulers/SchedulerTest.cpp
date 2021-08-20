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
