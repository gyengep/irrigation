#include <gmock/gmock.h>
#include "Schedulers/Scheduler.h"
#include "Utils/ToTimeT.h"

using namespace std;


TEST(Scheduler, Time) {
	tm tm = toCalendarTime(2016, 9, 11, 20, 59, 33, true);

	EXPECT_EQ(2016, tm.tm_year + 1900);
	EXPECT_EQ(9, tm.tm_mon + 1);
	EXPECT_EQ(11, tm.tm_mday);
	EXPECT_EQ(20, tm.tm_hour);
	EXPECT_EQ(59, tm.tm_min);
	EXPECT_EQ(33, tm.tm_sec);

	tm = toCalendarTime(2016, 9, 12, 23, 59, 33, true);

	EXPECT_EQ(2016, tm.tm_year + 1900);
	EXPECT_EQ(9, tm.tm_mon + 1);
	EXPECT_EQ(12, tm.tm_mday);
	EXPECT_EQ(23, tm.tm_hour);
	EXPECT_EQ(59, tm.tm_min);
	EXPECT_EQ(33, tm.tm_sec);

	tm = toCalendarTime(2016, 9, 17, 23, 59, 33, true);

	EXPECT_EQ(2016, tm.tm_year + 1900);
	EXPECT_EQ(9, tm.tm_mon + 1);
	EXPECT_EQ(17, tm.tm_mday);
	EXPECT_EQ(23, tm.tm_hour);
	EXPECT_EQ(59, tm.tm_min);
	EXPECT_EQ(33, tm.tm_sec);
}

TEST(SchedulerType, to_string) {
	EXPECT_EQ("weekly", to_string(SchedulerType::WEEKLY));
	EXPECT_EQ("periodic", to_string(SchedulerType::PERIODIC));
}

TEST(SchedulerType, to_string_Invalid) {
	int lastSchedulerTypeAsInt = static_cast<int>(SchedulerType::PERIODIC);
	SchedulerType schedulerType = static_cast<SchedulerType>(lastSchedulerTypeAsInt + 1);
	EXPECT_THROW(to_string(schedulerType), invalid_argument);
}
