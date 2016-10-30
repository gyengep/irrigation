/*
 * Document_test.cpp
 *
 *  Created on: 5 Jul 2016
 *      Author: pgyenge
 */

#include "common.h"
#include "Scheduler.h"

#include <cstring>


TEST(SpecifiedScheduler, enableDay) {
	SpecifiedScheduler scheduler;

	for (int i = 0; i < 7; i++) {
		EXPECT_FALSE(scheduler.days[i]);
		scheduler.enableDay(i, true);
		EXPECT_TRUE(scheduler.days[i]);
	}

	EXPECT_THROW(scheduler.enableDay(7, true), invalid_id);
}

TEST(SpecifiedScheduler, isDayEnabled) {
	SpecifiedScheduler scheduler;

	for (int i = 0; i < 7; i++) {
		EXPECT_FALSE(scheduler.isDayEnabled(i));
		scheduler.enableDay(i, true);
		EXPECT_TRUE(scheduler.isDayEnabled(i));
		scheduler.enableDay(i, false);
		EXPECT_FALSE(scheduler.isDayEnabled(i));
	}

	EXPECT_THROW(scheduler.isDayEnabled(7), invalid_id);
}

std::time_t toTime(int year, int month, int day, int hour, int min, int sec, bool dst) {

	EXPECT_TRUE(0 <= month && month < 12);
	EXPECT_TRUE(1 <= day && day < 32);
	EXPECT_TRUE(0 <= hour && hour < 24);
	EXPECT_TRUE(0 <= min && min < 60);
	EXPECT_TRUE(0 <= sec && sec < 60);

	std::tm tm;

	memset(&tm, 0, sizeof(tm));

	tm.tm_year = year - 1900;
	tm.tm_mon = month - 1;
	tm.tm_mday = day;
	tm.tm_hour = hour;
	tm.tm_min = min;
	tm.tm_sec = sec;
	tm.tm_isdst = dst ? 1 : 0;

	return std::mktime(&tm);
}

std::tm toCalendarTime(int year, int month, int day, int hour, int min, int sec, bool dst) {
	std::time_t t = toTime(year, month, day, hour, min, sec, dst);
	return *std::localtime(&t);
}

TEST(SpecifiedScheduler, Time) {
	std::tm tm = toCalendarTime(2016, 9, 11, 20, 59, 33, true);

	EXPECT_EQ(2016, tm.tm_year + 1900);
	EXPECT_EQ(9, tm.tm_mon + 1);
	EXPECT_EQ(11, tm.tm_mday);
	EXPECT_EQ(SUNDAY, tm.tm_wday);
	EXPECT_EQ(20, tm.tm_hour);
	EXPECT_EQ(59, tm.tm_min);
	EXPECT_EQ(33, tm.tm_sec);

	tm = toCalendarTime(2016, 9, 12, 23, 59, 33, true);

	EXPECT_EQ(2016, tm.tm_year + 1900);
	EXPECT_EQ(9, tm.tm_mon + 1);
	EXPECT_EQ(12, tm.tm_mday);
	EXPECT_EQ(MONDAY, tm.tm_wday);
	EXPECT_EQ(23, tm.tm_hour);
	EXPECT_EQ(59, tm.tm_min);
	EXPECT_EQ(33, tm.tm_sec);

	tm = toCalendarTime(2016, 9, 17, 23, 59, 33, true);

	EXPECT_EQ(2016, tm.tm_year + 1900);
	EXPECT_EQ(9, tm.tm_mon + 1);
	EXPECT_EQ(17, tm.tm_mday);
	EXPECT_EQ(SATURDAY, tm.tm_wday);
	EXPECT_EQ(23, tm.tm_hour);
	EXPECT_EQ(59, tm.tm_min);
	EXPECT_EQ(33, tm.tm_sec);
}

void checkDay(Scheduler& scheduler, bool requestedResult, int day) {
	std::tm tm;
	for (int hour = 0; hour < 24; hour++) {
		for (int min = 0; min < 60; min++) {
			for (int sec = 0; sec < 60; sec++) {
				tm = toCalendarTime(2016, 9, day, hour, min, sec, true);
				EXPECT_EQ(requestedResult, scheduler.isScheduled(tm));
			}
		}
	}
}

TEST(DISABLED_SpecifiedScheduler, isScheduled) {
	SpecifiedScheduler scheduler;

	scheduler.enableDay(WEDNESDAY, true);

	for (int day = 19; day < 26; day++) {
		checkDay(scheduler, 21 == day, day);
	}

	scheduler.enableDay(SUNDAY, true);
	scheduler.enableDay(WEDNESDAY, false);
	scheduler.enableDay(THURSDAY, true);

	for (int day = 19; day < 26; day++) {
		checkDay(scheduler, 22 == day || 25 == day, day);
	}
}
