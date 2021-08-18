#pragma once
#include <list>
#include <ostream>
#include "Utils/DateTime.h"


class TemperatureHistoryRepository {
public:

	struct Sample {
		const DateTime dateTime;
		const float temperature;

		Sample(const DateTime& dateTime, float temperature);

		// for testing
		bool operator== (const Sample& other) const;
		friend std::ostream& operator<<(std::ostream& os, const Sample& sample);
	};

	virtual ~TemperatureHistoryRepository() = default;
	virtual void removeOlder(const DateTime& dateTime) = 0;
	virtual void removeNewer(const DateTime& dateTime) = 0;
	virtual void add(const Sample& sample) = 0;
	virtual std::list<Sample> getBetween(const DateTime& from, const DateTime& to) const = 0;
	virtual std::list<Sample> getAll() const = 0;
};
