#pragma once
#include <ctime>
#include <chrono>
#include <deque>
#include <memory>
#include <mutex>
#include "Utils/Timer.h"
#include "TemperatureSensor.h"
#include "TemperatureHistory.h"

#ifndef FRIEND_TEST
#define FRIEND_TEST(a, b)
#endif

class CsvReaderFactory;
class CsvWriterFactory;

class TemperatureHistoryImpl : public TemperatureHistory, public TimerCallback {
	FRIEND_TEST(TemperatureHistoryTest, updateCache);
	FRIEND_TEST(TemperatureHistoryTest, removeNewer);
	FRIEND_TEST(TemperatureHistoryTest, removeOlder);
	FRIEND_TEST(TemperatureHistoryTest, getHistoryMin);
	FRIEND_TEST(TemperatureHistoryTest, getHistoryMax);
	FRIEND_TEST(TemperatureHistoryTest, getHistoryAvg);
	FRIEND_TEST(TemperatureHistoryTest, getHistoryNotFound);
	FRIEND_TEST(TemperatureHistoryTest, save);
	FRIEND_TEST(TemperatureHistoryTest, load);

public:

	struct TemperatureSample {
		std::time_t sampleTime;
		float value;

		TemperatureSample(const std::time_t& sampleTime, float value);

		// for testing
		bool operator== (const TemperatureSample& other) const;
		friend std::ostream& operator<<(std::ostream& os, const TemperatureSample& temperatureSample);
	};

private:

	mutable std::mutex mtx;
	const std::shared_ptr<TemperatureSensor> sensor;
	const std::chrono::seconds storedPeriod;
	const std::string fileName;
	const std::shared_ptr<CsvReaderFactory> csvReaderFactory;
	const std::shared_ptr<CsvWriterFactory> csvWriterFactory;

	std::deque<TemperatureSample> samples;

	void load();
	void save();
	void removeOlder(const std::time_t& rawTime);
	void removeNewer(const std::time_t& rawTime);
	void addTemperature(const std::time_t& rawTime, float temperature);
	void updateCache(const std::time_t& rawTime);

	static std::string temperatureToString(float value);

public:
	TemperatureHistoryImpl(
			const std::shared_ptr<TemperatureSensor>& sensor,
			const std::chrono::seconds& storePeriod,
			const std::string& fileName = "",
			const std::shared_ptr<CsvReaderFactory>& csvReaderFactory = std::shared_ptr<CsvReaderFactory>(),
			const std::shared_ptr<CsvWriterFactory>& csvWriterFactory = std::shared_ptr<CsvWriterFactory>()
		);

	virtual ~TemperatureHistoryImpl();
	virtual Values getHistoryValues(const std::time_t& from, const std::time_t& to) const override;

	void updateCache();

	void startTimer();
	void stopTimer();
	virtual void onTimer() override;

	// only for testing
	const std::deque<TemperatureSample> getContainer() const;
};
