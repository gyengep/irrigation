#include "common.h"
#include "StartTime.h"

#include <iomanip>


StartTime::StartTime(int hour, int min) :
	hour(hour),
	min(min)
{
}

StartTime& StartTime::operator= (const StartTime& other) {
	hour = other.hour;
	min = other.min;

	return *this;
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

std::ostream& operator<< (std::ostream& os, const StartTime& startTime) {
    return os << std::setfill('0') << std::setw(2) << startTime.getHour() << ":" << std::setfill('0') << std::setw(2) << startTime.getMin();
}
