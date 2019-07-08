#include "TemperatureForecastProvider.h"

using namespace std;


TemperatureForecastProvider::ValuesWithTimes::ValuesWithTimes(const std::time_t from, const std::time_t to, float min, float max) :
		ValuesWithTimes(
				chrono::system_clock::from_time_t(from),
				chrono::system_clock::from_time_t(to),
				min, max
			)
{}

TemperatureForecastProvider::ValuesWithTimes::ValuesWithTimes(
		const chrono::system_clock::time_point& from,
		const chrono::system_clock::time_point& to,
		float min,
		float max) :
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
	os << "from: " << chrono::system_clock::to_time_t(valuesWithTimes.from) << ", ";
	os << "to: " << chrono::system_clock::to_time_t(valuesWithTimes.to) << ", ";
	os << "min: " << valuesWithTimes.min << ", ";
	os << "max: " << valuesWithTimes.max;
	os << "}";
	return os;
}

