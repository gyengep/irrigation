#include "StartTime.h"

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
