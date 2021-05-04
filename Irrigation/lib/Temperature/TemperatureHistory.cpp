#include "TemperatureHistory.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

TemperatureHistory::Values::Values(float min, float max, float avg) :
	min(min),
	max(max),
	avg(avg)
{
}

bool TemperatureHistory::Values::operator== (const Values& other) const {
	return ((min == other.min) && (max == other.max));
}

ostream& operator<<(ostream& os, const TemperatureHistory::Values& values) {
	os << "Values{";
	os << "min: " << values.min << ", ";
	os << "max: " << values.max << ", ";
	os << "avg: " << values.avg;
	os << "}";
	return os;
}
