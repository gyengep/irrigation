#pragma once
#include <chrono>
#include <memory>
#include <string>
#include <ostream>
#include "Utils/Timer.h"

class CsvWriter;
class CsvWriterFactory;
class TemperatureHistory;


class TemperatureHistoryPersister : public TimerCallback {
	const std::shared_ptr<TemperatureHistory> temperatureHistory;
	const std::shared_ptr<CsvWriter> csvWriter;

	std::chrono::system_clock::time_point lastUpdateTime;
	std::chrono::duration<int64_t> period;
	std::unique_ptr<Timer> timer;

	static std::string temperatureToString(float value);
	static std::string timeToString(const std::chrono::system_clock::time_point& time);
	static std::shared_ptr<std::ostream> openFile(const std::string& fileName);

public:
	TemperatureHistoryPersister(
			const std::shared_ptr<TemperatureHistory>& temperatureStatistics,
			const std::shared_ptr<CsvWriterFactory>& csvWriterFactory,
			const std::string& fileName
		);

	TemperatureHistoryPersister(
			const std::shared_ptr<TemperatureHistory>& temperatureStatistics,
			const std::shared_ptr<CsvWriter>& csvWriter
		);

	virtual ~TemperatureHistoryPersister();

	void saveHistory(const std::chrono::system_clock::time_point& from, const std::chrono::system_clock::time_point& to);

	void startTimer(const std::chrono::duration<int64_t>& period);
	void stopTimer();
	virtual void onTimer() override;
};
