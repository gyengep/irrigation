#include "WeeklyScheduler.h"
#include "Utils/ToString.h"
#include <sstream>
#include <vector>

///////////////////////////////////////////////////////////////////////////////

bool WeeklyScheduler::operator== (const WeeklyScheduler& other) const {
	for (size_t i = 0; i < count; ++i) {
		if (isDayEnabled(i) != other.isDayEnabled(i)) {
			return false;
		}
	}

	return true;
}

std::ostream& operator<<(std::ostream& os, const WeeklyScheduler& weeklyScheduler) {
	os << weeklyScheduler.toString();
	return os;
}
