#pragma once
#include <chrono>
#include <memory>
#include <string>
#include <ostream>
#include "Utils/DateTime.h"
#include "Utils/Thread.h"
#include "TemperatureHistory.h"

class CsvWriterFactory;


class TemperatureHistoryLogger {
	const std::shared_ptr<TemperatureHistory> temperatureHistory;
	const std::shared_ptr<CsvWriterFactory> csvWriterFactory;

	std::unique_ptr<Thread> workerThread;
	std::chrono::seconds period;

	static std::string temperatureToString(float value);
	static std::shared_ptr<std::ostream> openFile(const std::string& fileName);

public:
	TemperatureHistoryLogger(
			const std::shared_ptr<TemperatureHistory>& temperatureHistory,
			const std::shared_ptr<CsvWriterFactory>& csvWriterFactory
		);

	~TemperatureHistoryLogger();

	void saveLog(const DateTime& from, const DateTime& to);

	void start();
	void stop();
};
