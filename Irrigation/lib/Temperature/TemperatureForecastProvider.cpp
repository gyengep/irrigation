#include "TemperatureForecastProvider.h"

using namespace std;


TemperatureForecastProvider::ValuesWithTimes::ValuesWithTimes(const DateTime& from, const DateTime& to, float min, float max) :
	from(from),
	to(to),
	min(min),
	max(max)
{
}

bool TemperatureForecastProvider::ValuesWithTimes::operator== (const ValuesWithTimes& other) const {
	return ((from == other.from) && (to == other.to) && (min == other.min) && (max == other.max));
}

ostream& operator<<(ostream& os, const TemperatureForecastProvider::ValuesWithTimes& valuesWithTimes) {
	os << "ValuesWithTimes{";
	os << "from: " << LocalDateTime(valuesWithTimes.from).toString() << ", ";
	os << "to: " << LocalDateTime(valuesWithTimes.to).toString() << ", ";
	os << "min: " << valuesWithTimes.min << ", ";
	os << "max: " << valuesWithTimes.max;
	os << "}";
	return os;
}

