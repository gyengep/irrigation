#pragma once
#include <ctime>
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

	std::time_t lastUpdateTime;
	std::chrono::seconds period;
	std::unique_ptr<Timer> timer;

	static std::string temperatureToString(float value);
	static std::shared_ptr<std::ostream> openFile(const std::string& fileName);

public:
	TemperatureHistoryPersister(
			const std::shared_ptr<TemperatureHistory>& temperatureHistory,
			const std::shared_ptr<CsvWriterFactory>& csvWriterFactory,
			const std::string& fileName
		);

	TemperatureHistoryPersister(
			const std::shared_ptr<TemperatureHistory>& temperatureHistory,
			const std::shared_ptr<CsvWriter>& csvWriter
		);

	virtual ~TemperatureHistoryPersister();

	void saveHistory(const std::time_t& from, const std::time_t& to);

	void startTimer(const std::chrono::seconds& period);
	void stopTimer();
	virtual void onTimer() override;
};
