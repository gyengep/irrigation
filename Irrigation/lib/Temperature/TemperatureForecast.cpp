#include "TemperatureForecast.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

TemperatureForecast::Values::Values(float min, float max) :
	min(min),
	max(max)
{
}

bool TemperatureForecast::Values::operator== (const Values& other) const {
	return ((min == other.min) && (max == other.max));
}

ostream& operator<<(ostream& os, const TemperatureForecast::Values& values) {
	os << "Values{";
	os << "min: " << values.min << ", ";
	os << "max: " << values.max;
	os << "}";
	return os;
}
