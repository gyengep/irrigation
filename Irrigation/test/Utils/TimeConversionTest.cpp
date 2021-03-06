#include <gmock/gmock.h>
#include <ctime>
#include "Utils/TimeConversion.h"

using namespace std;
using namespace testing;

string timeInfoToString(const tm& timeInfo) {
	char buffer[80];
	strftime(buffer, 80, "%F %T", &timeInfo);
	return string(buffer);
}

///////////////////////////////////////////////////////////////////////////////

TEST(ToCalendarTimeTest, YearMonthDay) {
	const tm timeinfo = toCalendarTime(2019, 10, 25);
	EXPECT_THAT(timeInfoToString(timeinfo), Eq("2019-10-25 00:00:00"));
}

TEST(ToCalendarTimeTest, YearMonthDayHourMinSec) {
	const tm timeinfo = toCalendarTime(2019, 10, 25, 5, 23, 47);
	EXPECT_THAT(timeInfoToString(timeinfo), Eq("2019-10-25 05:23:47"));
}

///////////////////////////////////////////////////////////////////////////////

TEST(ToLocalTimeTest, toLocalTime) {
	const tm timeinfo = toLocalTime(1571997323); // 2019-10-25T09:55:23+00:00
	EXPECT_THAT(timeInfoToString(timeinfo), Eq("2019-10-25 11:55:23"));
}

///////////////////////////////////////////////////////////////////////////////

TEST(FromUtcTimeTest, YearMonthDay) {
	const time_t rawTime = fromUtcTime(2019, 10, 25);
	EXPECT_THAT(rawTime, Eq(1571961600)); // 2019-10-25T00:00:00+00:00
}

TEST(FromUtcTimeTest, YearMonthDayHourMinSec) {
	const time_t rawTime = fromUtcTime(2019, 10, 25, 10, 1, 15);
	EXPECT_THAT(rawTime, Eq(1571997675)); // 2019-10-25T10:01:15+00:00
}

///////////////////////////////////////////////////////////////////////////////

TEST(FromLocalTimeTest, YearMonthDay) {
	const time_t rawTime = fromLocalTime(2019, 10, 25);
	EXPECT_THAT(rawTime, Eq(1571954400)); // 2019-10-24T22:00:00+00:00
}

TEST(FromLocalTimeTest, YearMonthDayHourMinSec) {
	const time_t rawTime = fromLocalTime(2019, 10, 25, 12, 1, 15);
	EXPECT_THAT(rawTime, Eq(1571997675)); // 2019-10-25T10:01:15+00:00
}

///////////////////////////////////////////////////////////////////////////////

TEST(ToLocalTimeStrTest, ToLocalTimeStr) {
	EXPECT_THAT(toLocalTimeStr(1571998073, "%F %T"), Eq("2019-10-25 12:07:53"));
}

///////////////////////////////////////////////////////////////////////////////

TEST(TimeConverterTest, getMillis) {
	TimeConverter timeConverter(
			chrono::milliseconds(240)
		);

	EXPECT_THAT(timeConverter.getDays(), Eq(0));
	EXPECT_THAT(timeConverter.getHours(), Eq(0));
	EXPECT_THAT(timeConverter.getMinutes(), Eq(0));
	EXPECT_THAT(timeConverter.getSeconds(), Eq(0));
	EXPECT_THAT(timeConverter.getMillis(), Eq(240));
}

TEST(TimeConverterTest, getSecond) {
	TimeConverter timeConverter(
			chrono::seconds(2) + chrono::milliseconds(240)
		);

	EXPECT_THAT(timeConverter.getDays(), Eq(0));
	EXPECT_THAT(timeConverter.getHours(), Eq(0));
	EXPECT_THAT(timeConverter.getMinutes(), Eq(0));
	EXPECT_THAT(timeConverter.getSeconds(), Eq(2));
	EXPECT_THAT(timeConverter.getMillis(), Eq(240));
}

TEST(TimeConverterTest, getMinutes) {
	TimeConverter timeConverter(
			chrono::minutes(7) + chrono::seconds(32) + chrono::milliseconds(54)
		);

	EXPECT_THAT(timeConverter.getDays(), Eq(0));
	EXPECT_THAT(timeConverter.getHours(), Eq(0));
	EXPECT_THAT(timeConverter.getMinutes(), Eq(7));
	EXPECT_THAT(timeConverter.getSeconds(), Eq(32));
	EXPECT_THAT(timeConverter.getMillis(), Eq(54));
}

TEST(TimeConverterTest, getHours) {
	TimeConverter timeConverter(
			chrono::hours(17) + chrono::minutes(12) + chrono::seconds(46) +
			chrono::milliseconds(54)
		);

	EXPECT_THAT(timeConverter.getDays(), Eq(0));
	EXPECT_THAT(timeConverter.getHours(), Eq(17));
	EXPECT_THAT(timeConverter.getMinutes(), Eq(12));
	EXPECT_THAT(timeConverter.getSeconds(), Eq(46));
	EXPECT_THAT(timeConverter.getMillis(), Eq(54));
}

TEST(TimeConverterTest, getDays) {
	TimeConverter timeConverter(
			chrono::hours(5) * 24 + chrono::hours(17) + chrono::minutes(12) +
			chrono::seconds(46) + chrono::milliseconds(54)
		);

	EXPECT_THAT(timeConverter.getDays(), Eq(5));
	EXPECT_THAT(timeConverter.getHours(), Eq(17));
	EXPECT_THAT(timeConverter.getMinutes(), Eq(12));
	EXPECT_THAT(timeConverter.getSeconds(), Eq(46));
	EXPECT_THAT(timeConverter.getMillis(), Eq(54));
}

///////////////////////////////////////////////////////////////////////////////

TEST(TimeConverterTest, absSeconds) {
	EXPECT_THAT(abs(chrono::seconds(5)), Eq(chrono::seconds(5)));
	EXPECT_THAT(abs(chrono::seconds(-5)), Eq(chrono::seconds(5)));
	EXPECT_THAT(abs(chrono::seconds(5000000)), Eq(chrono::seconds(5000000)));
	EXPECT_THAT(abs(chrono::seconds(-5000000)), Eq(chrono::seconds(5000000)));
}

TEST(TimeConverterTest, absMilliseconds) {
	EXPECT_THAT(abs(chrono::milliseconds(8)), Eq(chrono::milliseconds(8)));
	EXPECT_THAT(abs(chrono::milliseconds(-8)), Eq(chrono::milliseconds(8)));
	EXPECT_THAT(abs(chrono::milliseconds(80000000000)), Eq(chrono::milliseconds(80000000000)));
	EXPECT_THAT(abs(chrono::milliseconds(-80000000000)), Eq(chrono::milliseconds(80000000000)));
}

///////////////////////////////////////////////////////////////////////////////

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

TEST(GetElapsedDaysSinceEpochTest, getHoursWinter) {
	tm timeinfo = toCalendarTime(2018, 12, 10);
	const int elapsedDays = getElapsedDaysSinceEpoch(timeinfo);

	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {
				timeinfo = toCalendarTime(2018, 12, 10, hour, min, sec);
				ASSERT_THAT(getElapsedDaysSinceEpoch(timeinfo), elapsedDays);
			}
		}
	}

	timeinfo = toCalendarTime(2018, 12, 9, 23, 59, 59);
	ASSERT_THAT(getElapsedDaysSinceEpoch(timeinfo), elapsedDays - 1);

	timeinfo = toCalendarTime(2018, 12, 11, 0, 0, 0);
	ASSERT_THAT(getElapsedDaysSinceEpoch(timeinfo), elapsedDays + 1);
}

TEST(GetElapsedDaysSinceEpochTest, getHoursSummer) {
	tm timeinfo = toCalendarTime(2018, 6, 10);
	const int elapsedDays = getElapsedDaysSinceEpoch(timeinfo);

	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {
				timeinfo = toCalendarTime(2018, 6, 10, hour, min, sec);
				ASSERT_THAT(getElapsedDaysSinceEpoch(timeinfo), elapsedDays);
			}
		}
	}

	timeinfo = toCalendarTime(2018, 6, 9, 23, 59, 59);
	ASSERT_THAT(getElapsedDaysSinceEpoch(timeinfo), elapsedDays - 1);

	timeinfo = toCalendarTime(2018, 6, 11, 0, 0, 0);
	ASSERT_THAT(getElapsedDaysSinceEpoch(timeinfo), elapsedDays + 1);
}

///////////////////////////////////////////////////////////////////////////////

TEST(TimePeriodTest, getCurrentPeriod) {
	const time_t currentTime = fromLocalTime(2019, 7, 5, 13, 31, 0);

	EXPECT_THAT(
			getCurrentPeriod(currentTime, chrono::minutes(10)),
			Pair(
				fromLocalTime(2019, 7, 5, 13, 30, 0),
				fromLocalTime(2019, 7, 5, 13, 39, 59)
			));

	EXPECT_THAT(
			getCurrentPeriod(currentTime, chrono::hours(1)),
			Pair(
				fromLocalTime(2019, 7, 5, 13, 0, 0),
				fromLocalTime(2019, 7, 5, 13, 59, 59)
			));
}

TEST(TimePeriodTest, getPreviousPeriod) {
	const time_t currentTime = fromLocalTime(2019, 7, 5, 13, 31, 0);

	EXPECT_THAT(
			getPreviousPeriod(currentTime, chrono::minutes(10)),
			Pair(
				fromLocalTime(2019, 7, 5, 13, 20, 0),
				fromLocalTime(2019, 7, 5, 13, 29, 59)
			));

	EXPECT_THAT(
			getPreviousPeriod(currentTime, chrono::hours(1)),
			Pair(
				fromLocalTime(2019, 7, 5, 12, 0, 0),
				fromLocalTime(2019, 7, 5, 12, 59, 59)
			));
}
