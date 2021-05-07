#pragma once
#include <chrono>
#include <ctime>
#include <string>
#include <utility>


std::tm toCalendarTime(int year, int month, int day, int hour = 0, int min = 0, int sec = 0);
std::tm toLocalTime(const std::time_t& rawTime);
std::time_t fromUtcTime(int year, int month, int day, int hour = 0, int min = 0, int sec = 0);
std::time_t fromLocalTime(int year, int month, int day, int hour = 0, int min = 0, int sec = 0);
std::string toLocalTimeStr(const std::time_t& rawTime, const char* formatStr);

///////////////////////////////////////////////////////////////////////////////

class TimeConverter {
	typedef std::chrono::duration<int64_t, std::ratio<24 * 3600>> Days;

	Days days;
	std::chrono::hours hours;
	std::chrono::minutes minutes;
	std::chrono::seconds seconds;
	std::chrono::milliseconds millis;

public:
	TimeConverter(const std::chrono::milliseconds& ms);

	int64_t getDays() const { return days.count(); }
	int64_t getHours() const { return hours.count(); }
	int64_t getMinutes() const { return minutes.count(); }
	int64_t getSeconds() const { return seconds.count(); }
	int64_t getMillis() const { return millis.count(); }
};


std::chrono::seconds abs(const std::chrono::seconds& t);
std::chrono::milliseconds abs(const std::chrono::milliseconds& t);

unsigned getElapsedDaysSinceEpoch(const std::tm& timeinfo);

std::pair<std::time_t, std::time_t> getPreviousPeriod(const std::time_t& rawTime, const std::chrono::seconds& periodLength);
std::pair<std::time_t, std::time_t> getCurrentPeriod(const std::time_t& rawTime, const std::chrono::seconds& periodLength);
