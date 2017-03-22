#include "common.h"
#include "StartTime.h"


StartTime::StartTime(int hour, int min) :
	hour(hour),
	min(min)
{
}

bool StartTime::operator== (const StartTime& other) const {
	return (hour == other.hour) && (min == other.min);
}

bool StartTime::operator< (const StartTime& other) const {
	if (hour < other.hour) {
		return true;
	} else if (hour > other.hour) {
		return false;
	} else {
		return (min < other.min);
	}
}
