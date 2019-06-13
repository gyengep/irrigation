#pragma once
#include <chrono>
#include <memory>
#include <mutex>
#include <queue>
#include "TemperatureStatistics.h"

class CsvReaderFactory;
class CsvWriterFactory;


class TemperatureStatisticsImpl : public TemperatureStatistics {
public:

	struct TemperatureSample {
		time_t sampleTime;
		float value;

		TemperatureSample(time_t sampleTime, float value);

		// for testing
		bool operator== (const TemperatureSample& other) const;
		friend std::ostream& operator<<(std::ostream& os, const TemperatureSample& temperatureSample);
	};

private:

	struct GreaterThan {
		const time_t rawTime;

		GreaterThan(time_t rawTime);
		bool operator() (const TemperatureSample& sample);
	};

	const std::string fileName;
	const std::shared_ptr<CsvReaderFactory> csvReaderFactory;
	const std::shared_ptr<CsvWriterFactory> csvWriterFactory;

	const int64_t storedSeconds;
	std::deque<TemperatureSample> samples;
	mutable std::mutex mtx;

	void load();
	void save();
	void removeOlder(std::time_t rawTime);
	void removeNewer(std::time_t rawTime);

	static std::string temperatureToString(float value);

public:
	TemperatureStatisticsImpl(const std::chrono::duration<int64_t>& storePeriod);
	TemperatureStatisticsImpl(
			const std::chrono::duration<int64_t>& storePeriod,
			const std::string& fileName,
			const std::shared_ptr<CsvReaderFactory>& csvReaderFactory,
			const std::shared_ptr<CsvWriterFactory>& csvWriterFactory
			);

	virtual ~TemperatureStatisticsImpl();

	void addTemperature(std::time_t rawTime, float temperature) override;
	virtual StatisticsValues getStatistics(std::time_t from, std::time_t to) override;

	// only for testing
	const std::deque<TemperatureSample> getSamples() const;
};
