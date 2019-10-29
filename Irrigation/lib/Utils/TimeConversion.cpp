#include "TimeConversion.h"
#include <cstring>
#include <stdexcept>

using namespace std;
using namespace std::chrono;

///////////////////////////////////////////////////////////////////////////////

TimeConverter::TimeConverter(const milliseconds& ms) {
	days = duration_cast<Days>(ms);
	hours = duration_cast<chrono::hours>(ms - days);
	minutes = duration_cast<chrono::minutes>(ms - days - hours);
	seconds = duration_cast<chrono::seconds>(ms - days - hours - minutes);
	millis = duration_cast<chrono::milliseconds>(ms - days - hours - minutes - seconds);
}

///////////////////////////////////////////////////////////////////////////////

seconds abs(const seconds& t) {
	return seconds(abs(t.count()));
}

milliseconds abs(const milliseconds& t) {
	return milliseconds(abs(t.count()));
}

///////////////////////////////////////////////////////////////////////////////

tm toCalendarTime(int year, int month, int day, int hour, int min, int sec) {

	if ((year < 1900) ||
		(month < 1 || month > 12) ||
		(day < 1 || day > 31) ||
		(hour < 0 || hour >= 24) ||
		(min < 0 || min >= 60) ||
		(sec < 0 || sec >= 60))
	{
		throw runtime_error(string("Invalid date:") +
				" year: " + to_string(year) +
				" month: " + to_string(month) +
				" day: " + to_string(day) +
				" hour: " + to_string(hour) +
				" min: " + to_string(min) +
				" sec: " + to_string(sec));
	}

	struct tm timeinfo;
	memset(&timeinfo, 0, sizeof(timeinfo));

	timeinfo.tm_year = year - 1900;
	timeinfo.tm_mon = month - 1;
	timeinfo.tm_mday = day;
	timeinfo.tm_hour = hour;
	timeinfo.tm_min = min;
	timeinfo.tm_sec = sec;
	timeinfo.tm_isdst = -1;

	return timeinfo;
}

std::tm toLocalTime(const std::time_t& rawTime) {
	struct tm timeinfo;
	localtime_r(&rawTime, &timeinfo);
	return timeinfo;
}

std::time_t fromUtcTime(int year, int month, int day, int hour, int min, int sec) {
	struct tm calendarTime = toCalendarTime(year, month, day, hour, min, sec);
	return timegm(&calendarTime);
}

std::time_t fromLocalTime(int year, int month, int day, int hour, int min, int sec) {
	struct tm calendarTime = toCalendarTime(year, month, day, hour, min, sec);
	return mktime(&calendarTime);
}

std::string toLocalTimeStr(const time_t& rawTime, const char* formatStr) {
	char buffer[100];
	struct tm timeinfo;

	strftime(buffer, 100, formatStr, localtime_r(&rawTime, &timeinfo));
	return string(buffer);
}

///////////////////////////////////////////////////////////////////////////////

unsigned getElapsedDaysSinceEpoch(const tm& timeinfo) {
	struct tm timeinfoCopy = timeinfo;
	const time_t rawtime = timegm(&timeinfoCopy);
	if (rawtime == (time_t)-1) {
		throw runtime_error(string("Invalid timeinfo:") +
				" year: " + to_string(timeinfo.tm_year) +
				" month: " + to_string(timeinfo.tm_mon) +
				" day: " + to_string(timeinfo.tm_mday) +
				" hour: " + to_string(timeinfo.tm_hour) +
				" min: " + to_string(timeinfo.tm_min) +
				" sec: " + to_string(timeinfo.tm_sec));
	}

	return rawtime / (60 * 60 * 24);
}

///////////////////////////////////////////////////////////////////////////////

pair<time_t, time_t> getPreviousPeriod(const time_t& rawTime, const seconds& periodLength) {
	const time_t periodInSeconds = periodLength.count();

	const time_t from = ((rawTime / periodInSeconds) - 1) * periodInSeconds;
	const time_t to = (rawTime / periodInSeconds) * periodInSeconds - 1;

	return make_pair(from, to);
}

pair<time_t, time_t> getCurrentPeriod(const time_t& rawTime, const seconds& periodLength) {
	const time_t periodInSeconds = periodLength.count();

	const time_t from = (rawTime / periodInSeconds) * periodInSeconds;
	const time_t to = ((rawTime / periodInSeconds) + 1) * periodInSeconds - 1;

	return make_pair(from, to);
}
