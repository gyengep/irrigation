#pragma once
#include <chrono>
#include "Utils/Timer.h"


class TemperatureStatistics : public TimerCallback {
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
	virtual ~TemperatureStatistics() = default;
	virtual Values getStatisticsValues(std::time_t from, std::time_t to) const = 0;
	virtual void onTimer() override {}
};
