#include "WeeklyScheduler.h"
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
