#include <gmock/gmock.h>
#include "Schedulers/Scheduler.h"

using namespace std;
using namespace testing;


TEST(SchedulerTypeTest, to_string) {
	EXPECT_THAT(to_string(SchedulerType::WEEKLY), Eq("weekly"));
	EXPECT_THAT(to_string(SchedulerType::PERIODIC), Eq("periodic"));
	EXPECT_THAT(to_string(SchedulerType::EVERY_DAY), Eq("every_day"));
	EXPECT_THAT(to_string(SchedulerType::FIXED_AMOUNT), Eq("fixed_amount"));
	EXPECT_THAT(to_string(SchedulerType::FIXED_PERIOD), Eq("fixed_period"));
}

TEST(SchedulerTypeTest, to_string_Invalid) {
	int lastSchedulerTypeAsInt = static_cast<int>(SchedulerType::FIXED_PERIOD);
	SchedulerType schedulerType = static_cast<SchedulerType>(lastSchedulerTypeAsInt + 1);
	EXPECT_THROW(to_string(schedulerType), invalid_argument);
}
