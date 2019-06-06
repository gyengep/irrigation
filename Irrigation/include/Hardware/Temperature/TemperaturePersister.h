#pragma once
#include <chrono>
#include <memory>
#include <string>
#include <ostream>

class CsvWriter;
class CsvWriterFactory;
class TemperatureStatistics;


class TemperaturePersister {
	const time_t periodInSeconds;
	const std::shared_ptr<TemperatureStatistics> temperatureStatistics;
	const std::shared_ptr<std::ostream> output;
	const std::shared_ptr<CsvWriter> csvWriter;

	time_t lastUpdate;

	void persistData();

	static std::string timeToString(std::time_t time);
	static std::string temperatureToString(float value);
	static std::shared_ptr<std::ostream> openFile(const std::string& fileName);

public:
	TemperaturePersister(
			const std::chrono::duration<int64_t>& period,
			const std::shared_ptr<TemperatureStatistics>& temperatureStatistics,
			const std::string& fileName,
			const std::shared_ptr<CsvWriterFactory>& csvWriterFactory
		);

	virtual ~TemperaturePersister();

	void periodicUpdate();
};
