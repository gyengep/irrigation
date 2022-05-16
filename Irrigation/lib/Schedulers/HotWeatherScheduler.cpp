#include "HotWeatherScheduler.h"
#include <sstream>

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, const HotWeatherScheduler& scheduler) {
	os << scheduler.toString();
	return os;
}
