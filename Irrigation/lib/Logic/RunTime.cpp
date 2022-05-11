#include "RunTime.h"
#include <iomanip>
#include <sstream>

///////////////////////////////////////////////////////////////////////////////

bool RunTime::operator== (const RunTime& other) const {
	return (getMilliSeconds() == other.getMilliSeconds());
}

std::string to_string(const RunTime& runTime) {
	std::ostringstream oss;
	oss << runTime;
	return oss.str();
}

std::ostream& operator<<(std::ostream& os, const RunTime& runTime) {
	os << std::setfill('0') << std::setw(2) << (runTime.getSeconds() / 60) << ":";
	os << std::setfill('0') << std::setw(2) << (runTime.getSeconds() % 60);
	return os;
}
