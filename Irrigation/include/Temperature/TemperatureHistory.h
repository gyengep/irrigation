#pragma once
#include <ctime>


class TemperatureHistory {
public:

	struct Values {
		float min, max, avg;

		Values(float min, float max, float avg) :
			min(min),
			max(max),
			avg(avg)
		{
		}
	};

	virtual ~TemperatureHistory() = default;
	virtual Values getTemperatureHistory(const std::time_t& from, const std::time_t& to) const = 0;
};
