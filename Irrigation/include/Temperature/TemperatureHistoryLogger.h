#pragma once
#include <ctime>
#include <chrono>
#include <memory>
#include <string>
#include <ostream>
#include "Utils/Thread.h"
#include "TemperatureHistory.h"

class CsvWriterFactory;


class TemperatureHistoryLogger {
	const std::shared_ptr<TemperatureHistory> temperatureHistory;
	const std::shared_ptr<CsvWriterFactory> csvWriterFactory;

	std::unique_ptr<Thread> workerThread;
	std::time_t lastUpdateTime;
	std::chrono::seconds period;

	static std::string temperatureToString(float value);
	static std::shared_ptr<std::ostream> openFile(const std::string& fileName);

public:
	TemperatureHistoryLogger(
			const std::shared_ptr<TemperatureHistory>& temperatureHistory,
			const std::shared_ptr<CsvWriterFactory>& csvWriterFactory
		);

	~TemperatureHistoryLogger();

	void saveLog(const std::time_t& from, const std::time_t& to);

	void start();
	void stop();
};
