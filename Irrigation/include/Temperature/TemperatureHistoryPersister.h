#pragma once
#include <ctime>
#include <list>
#include <ostream>


class TemperatureHistoryPersister {
public:

	struct Sample {
		const std::time_t rawTime;
		const float temperature;

		Sample(const std::time_t& rawTime, float temperature);

		// for testing
		bool operator== (const Sample& other) const;
		friend std::ostream& operator<<(std::ostream& os, const Sample& sample);
	};

	virtual ~TemperatureHistoryPersister() = default;
	virtual void removeOlder(const std::time_t& rawTime) = 0;
	virtual void removeNewer(const std::time_t& rawTime) = 0;
	virtual void add(const Sample& sample) = 0;
	virtual std::list<Sample> getBetween(const std::time_t& from, const std::time_t& to) const = 0;
	virtual std::list<Sample> getAll() const = 0;
};
