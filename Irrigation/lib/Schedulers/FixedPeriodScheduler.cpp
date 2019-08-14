#include "FixedPeriodScheduler.h"
#include "Logger/Logger.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

FixedPeriodScheduler::FixedPeriodScheduler(const shared_ptr<TemperatureForecast>& temperatureForecast, const shared_ptr<TemperatureHistory>& temperatureHistory) :
	TemperatureDependentScheduler(temperatureForecast, temperatureHistory)
{
}

FixedPeriodScheduler::~FixedPeriodScheduler() {
}

string to_string(const FixedPeriodScheduler& scheduler) {
	ostringstream oss;
	oss << scheduler;
	return oss.str();
}

ostream& operator<<(ostream& os, const FixedPeriodScheduler& scheduler) {
	os << "FixedPeriodScheduler{";
	os << static_cast<const TemperatureDependentScheduler&>(scheduler);
	os << "}";
	return os;
}
