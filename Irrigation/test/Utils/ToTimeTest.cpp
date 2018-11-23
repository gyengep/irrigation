#include <gmock/gmock.h>
#include <ctime>
#include "Utils/ToTimeT.h"

using namespace std;
using namespace testing;


TEST(ToCalendarTimeTest, YearMonthDay) {
	char mbstr[100];
	tm timeinfo;

	timeinfo = toCalendarTime(2018, 11, 23);
	strftime(mbstr, sizeof(mbstr), "%Y-%m-%d %H:%M:%S", &timeinfo);
	EXPECT_THAT(mbstr, StrEq("2018-11-23 00:00:00"));
}

TEST(ToCalendarTimeTest, YearMonthDayHourMinSec) {
	char mbstr[100];
	tm timeinfo;

	timeinfo = toCalendarTime(2018, 11, 23, 5, 23, 47);
	strftime(mbstr, sizeof(mbstr), "%Y-%m-%d %H:%M:%S", &timeinfo);
	EXPECT_THAT(mbstr, StrEq("2018-11-23 05:23:47"));

	timeinfo = toCalendarTime(2018, 6, 23, 5, 23, 47);
	strftime(mbstr, sizeof(mbstr), "%Y-%m-%d %H:%M:%S", &timeinfo);
	EXPECT_THAT(mbstr, StrEq("2018-06-23 05:23:47"));
}

