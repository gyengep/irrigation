#pragma once
#include <ostream>
#include "Utils/DateTime.h"


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
	virtual Values getTemperatureHistory(const DateTime& from, const DateTime& to) const = 0;
};
