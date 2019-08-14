#include "FixedAmountScheduler.h"
#include "Logger/Logger.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

FixedAmountScheduler::FixedAmountScheduler(const shared_ptr<TemperatureForecast>& temperatureForecast, const shared_ptr<TemperatureHistory>& temperatureHistory) :
	TemperatureDependentScheduler(temperatureForecast, temperatureHistory)
{
	setMinAdjustment(100);
}

FixedAmountScheduler::~FixedAmountScheduler() {
}

string to_string(const FixedAmountScheduler& scheduler) {
	ostringstream oss;
	oss << scheduler;
	return oss.str();
}

ostream& operator<<(ostream& os, const FixedAmountScheduler& scheduler) {
	os << "FixedAmountScheduler{";
	os << static_cast<const TemperatureDependentScheduler&>(scheduler);
	os << "}";
	return os;
}
