#include <gmock/gmock.h>
#include <ctime>
#include "Utils/TimeConversion.h"

using namespace std;
using namespace testing;


TEST(ToCalendarTimeTest, YearMonthDay) {
	char mbstr[100];
	tm timeinfo = toCalendarTime(2018, 11, 23);
	strftime(mbstr, sizeof(mbstr), "%Y-%m-%d %H:%M:%S", &timeinfo);
	EXPECT_THAT(mbstr, StrEq("2018-11-23 00:00:00"));
}

TEST(ToCalendarTimeTest, YearMonthDayHourMinSec) {
	char mbstr[100];
	tm timeinfo;

	timeinfo = toCalendarTime(2018, 11, 23, 5, 23, 47);
	strftime(mbstr, sizeof(mbstr), "%Y-%m-%d %H:%M:%S", &timeinfo);
	EXPECT_THAT(mbstr, StrEq("2018-11-23 05:23:47"));
}


TEST(GetElapsedDaysSinceEpochTest, getDays) {
	const int monthLengthsLeapYears[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	const int monthLengthsNotLeapYears[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	int elapsedDays = 0;

	for (int year = 1970; year < 2038; year++) {
		const int* monthLengths;

		if (0 == (year % 4)) {
			monthLengths = monthLengthsLeapYears;
		} else {
			monthLengths = monthLengthsNotLeapYears;
		}

		for (int month = 1; month <= 12; month++) {
			for (int dayOfMonth = 1; dayOfMonth <= monthLengths[month - 1]; dayOfMonth++) {
				const tm timeinfo = toCalendarTime(year, month, dayOfMonth);
				ASSERT_THAT(getElapsedDaysSinceEpoch(timeinfo), elapsedDays);
				elapsedDays++;
			}
		}
	}
}

TEST(GetElapsedDaysSinceEpochTest, getHours) {
	const tm timeinfo = toCalendarTime(2018, 11, 24);
	const int elapsedDays = getElapsedDaysSinceEpoch(timeinfo);

	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {
				const tm timeinfo = toCalendarTime(2018, 11, 24, hour, min, sec);
				ASSERT_THAT(getElapsedDaysSinceEpoch(timeinfo), elapsedDays);
			}
		}
	}
}
