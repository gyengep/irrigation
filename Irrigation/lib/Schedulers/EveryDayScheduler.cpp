#include "EveryDayScheduler.h"
#include <sstream>

///////////////////////////////////////////////////////////////////////////////

bool EveryDayScheduler::operator== (const EveryDayScheduler& other) const {
	return true;
}

std::ostream& operator<<(std::ostream& os, const EveryDayScheduler& scheduler) {
	os << scheduler.toString();
	return os;
}
