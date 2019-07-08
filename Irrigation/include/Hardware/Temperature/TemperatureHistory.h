#pragma once
#include <chrono>


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

public:
	virtual ~TemperatureHistory() = default;
	virtual Values getHistoryValues(const std::chrono::system_clock::time_point& from, const std::chrono::system_clock::time_point& to) const = 0;
};
