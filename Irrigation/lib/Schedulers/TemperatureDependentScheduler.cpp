#include "TemperatureDependentScheduler.h"
#include <sstream>

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, const TemperatureDependentScheduler& scheduler) {
	os << scheduler.toString();
	return os;
}
