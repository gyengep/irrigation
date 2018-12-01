#include "ToTimeT.h"
#include <cstring>
#include <stdexcept>

using namespace std;

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

	tm timeinfo;
	memset(&timeinfo, 0, sizeof(timeinfo));

	timeinfo.tm_year = year - 1900;
	timeinfo.tm_mon = month - 1;
	timeinfo.tm_mday = day;
	timeinfo.tm_hour = hour;
	timeinfo.tm_min = min;
	timeinfo.tm_sec = sec;

	return timeinfo;
}

tm toCalendarTime(int year, int month, int day) {
	return toCalendarTime(year, month, day, 0, 0, 0);
}