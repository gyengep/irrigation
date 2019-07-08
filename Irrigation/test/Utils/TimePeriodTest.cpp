#include <gmock/gmock.h>
#include "Utils/TimeConversion.h"
#include "Utils/TimePeriod.h"

using namespace std;
using namespace testing;


chrono::system_clock::time_point toSystemTime(int year, int month, int day, int hour, int min, int sec) {
	tm timeinfo = toCalendarTime(year, month, day, hour, min, sec);
	return chrono::system_clock::from_time_t(mktime(&timeinfo));

}

TEST(TimePeriodTest, getCurrentPeriod) {
	const auto currentTime = toSystemTime(2019, 7, 5, 13, 31, 0);

	EXPECT_THAT(
			getCurrentPeriod(currentTime, chrono::minutes(10)),
			Pair(
				toSystemTime(2019, 7, 5, 13, 30, 0),
				toSystemTime(2019, 7, 5, 13, 39, 59)
			));

	EXPECT_THAT(
			getCurrentPeriod(currentTime, chrono::hours(1)),
			Pair(
				toSystemTime(2019, 7, 5, 13, 0, 0),
				toSystemTime(2019, 7, 5, 13, 59, 59)
			));
}

TEST(TimePeriodTest, getPreviousPeriod) {
	const auto currentTime = toSystemTime(2019, 7, 5, 13, 31, 0);

	EXPECT_THAT(
			getPreviousPeriod(currentTime, chrono::minutes(10)),
			Pair(
				toSystemTime(2019, 7, 5, 13, 20, 0),
				toSystemTime(2019, 7, 5, 13, 29, 59)
			));

	EXPECT_THAT(
			getPreviousPeriod(currentTime, chrono::hours(1)),
			Pair(
				toSystemTime(2019, 7, 5, 12, 0, 0),
				toSystemTime(2019, 7, 5, 12, 59, 59)
			));
}
