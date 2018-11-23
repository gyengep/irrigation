#include <gmock/gmock.h>
#include "Schedulers/Scheduler.h"
#include "Utils/ToTimeT.h"

using namespace std;


TEST(SchedulerType, to_string) {
	EXPECT_EQ("weekly", to_string(SchedulerType::WEEKLY));
	EXPECT_EQ("periodic", to_string(SchedulerType::PERIODIC));
}

TEST(SchedulerType, to_string_Invalid) {
	int lastSchedulerTypeAsInt = static_cast<int>(SchedulerType::PERIODIC);
	SchedulerType schedulerType = static_cast<SchedulerType>(lastSchedulerTypeAsInt + 1);
	EXPECT_THROW(to_string(schedulerType), invalid_argument);
}
