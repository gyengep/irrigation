#pragma once
#include <ctime>
#include <ostream>


class TemperatureHistory {
public:

	struct Values {
		const float min;
		const float max;
		const float avg;

		Values(float min, float max, float avg);

		// for testing
		bool operator== (const Values& other) const;
		friend std::ostream& operator<<(std::ostream& os, const Values& values);
	};

	virtual ~TemperatureHistory() = default;
	virtual Values getTemperatureHistory(const std::time_t& from, const std::time_t& to) const = 0;
};
