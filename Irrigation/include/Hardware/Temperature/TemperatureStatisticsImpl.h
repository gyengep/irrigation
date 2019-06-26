#pragma once
#include <chrono>
#include <deque>
#include <memory>
#include <mutex>
#include "TemperatureSensor.h"
#include "TemperatureStatistics.h"
#include "Utils/Timer.h"

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

	mutable std::mutex mtx;
	const std::string fileName;
	const std::shared_ptr<CsvReaderFactory> csvReaderFactory;
	const std::shared_ptr<CsvWriterFactory> csvWriterFactory;
	const std::chrono::seconds::rep storedSeconds;
	const std::shared_ptr<TemperatureSensor> sensor;
	std::deque<TemperatureSample> samples;

	void load();
	void save();
	void removeOlder(std::time_t rawTime);
	void removeNewer(std::time_t rawTime);

	static std::string temperatureToString(float value);

public:
	TemperatureStatisticsImpl(
			const std::chrono::seconds& storePeriod,
			const std::string& fileName = "",
			const std::shared_ptr<CsvReaderFactory>& csvReaderFactory = std::shared_ptr<CsvReaderFactory>(),
			const std::shared_ptr<CsvWriterFactory>& csvWriterFactory = std::shared_ptr<CsvWriterFactory>(),
			const std::shared_ptr<TemperatureSensor>& sensor = std::shared_ptr<TemperatureSensor>()
		);

	virtual ~TemperatureStatisticsImpl();

	void addTemperature(std::time_t rawTime, float temperature);
	virtual Values getStatisticsValues(std::time_t from, std::time_t to) const override;
	virtual void onTimer() override;

	// only for testing
	const std::deque<TemperatureSample> getContainer() const;
};
