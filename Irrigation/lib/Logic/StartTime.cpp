#include "StartTime.h"
#include <iomanip>
#include <sstream>

///////////////////////////////////////////////////////////////////////////////

bool StartTime::operator< (const StartTime& other) const {
	return ((60 * getHours() + getMinutes()) < (60 * other.getHours() + other.getMinutes()));
}

bool StartTime::operator== (const StartTime& other) const {
	return ((getHours() == other.getHours()) &&
			(getMinutes() == other.getMinutes()));
}

bool StartTime::equals(unsigned hour, unsigned minute, unsigned second) const {
	return (getHours() == hour &&
			getMinutes() == minute &&
			0 == second);
}

std::string to_string_short(const StartTime& startTime) {
	std::ostringstream oss;
	oss << std::setfill('0') << std::setw(2) << startTime.getHours() << ":";
	oss << std::setfill('0') << std::setw(2) << startTime.getMinutes();
	return oss.str();
}

std::string to_string(const StartTime& startTime) {
	std::ostringstream oss;
	oss << startTime;
	return oss.str();
}

std::ostream& operator<<(std::ostream& os, const StartTime& startTime) {
	os << "StartTime{";
	os << std::setfill('0') << std::setw(2) << startTime.getHours() << ":";
	os << std::setfill('0') << std::setw(2) << startTime.getMinutes();
	os << "}";
	return os;
}
