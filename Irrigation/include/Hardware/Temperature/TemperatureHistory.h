#pragma once
#include <chrono>
#include <memory>
#include <string>
#include <ostream>

class CsvWriter;
class CsvWriterFactory;
class TemperatureStatistics;


class TemperatureHistory {
	const time_t periodInSeconds;
	const std::shared_ptr<TemperatureStatistics> temperatureStatistics;
	const std::shared_ptr<CsvWriter> csvWriter;

	time_t lastUpdate;

	void persistData();

	static std::string temperatureToString(float value);
	static std::shared_ptr<std::ostream> openFile(const std::string& fileName);
	static std::string timeToString(std::time_t time);

public:
	TemperatureHistory(
			const std::chrono::duration<int64_t>& period,
			const std::shared_ptr<TemperatureStatistics>& temperatureStatistics,
			const std::string& fileName,
			const std::shared_ptr<CsvWriterFactory>& csvWriterFactory
		);

	TemperatureHistory(
			const std::chrono::duration<int64_t>& period,
			const std::shared_ptr<TemperatureStatistics>& temperatureStatistics,
			const std::shared_ptr<CsvWriter>& csvWriter,
			const std::time_t currentTime
		);

	virtual ~TemperatureHistory();

	void periodicUpdate();
	void periodicUpdate(std::time_t time);
};
