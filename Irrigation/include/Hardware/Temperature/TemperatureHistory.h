#pragma once
#include <chrono>
#include <memory>
#include <string>
#include <ostream>
#include "Utils/Timer.h"

class CsvWriter;
class CsvWriterFactory;
class TemperatureStatistics;


class TemperatureHistory : public TimerCallback {
	const std::chrono::seconds::rep periodInSeconds;
	const std::shared_ptr<TemperatureStatistics> temperatureStatistics;
	const std::shared_ptr<CsvWriter> csvWriter;

	time_t lastUpdate;

	static std::string temperatureToString(float value);
	static std::string timeToString(std::time_t time);
	static std::shared_ptr<std::ostream> openFile(const std::string& fileName);

public:
	TemperatureHistory(
			const std::shared_ptr<TemperatureStatistics>& temperatureStatistics,
			const std::chrono::seconds& period,
			const std::string& fileName,
			const std::shared_ptr<CsvWriterFactory>& csvWriterFactory
		);

	TemperatureHistory(
			const std::shared_ptr<TemperatureStatistics>& temperatureStatistics,
			const std::chrono::seconds& period,
			const std::shared_ptr<CsvWriter>& csvWriter,
			const std::time_t currentTime
		);

	virtual ~TemperatureHistory();

	void periodicUpdate(std::time_t time = std::time(nullptr));

	virtual void onTimer() override;
};
