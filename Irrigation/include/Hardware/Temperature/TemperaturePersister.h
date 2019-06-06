#pragma once
#include <ctime>
#include <deque>
#include <mutex>
#include <string>
#include <ostream>
#include <vector>
#include "Utils/Timer.h"

class CsvWriter;
class CsvWriterFactory;
class TemperatureStatistics;


class TemperaturePersister : public TimerCallback {
	const std::shared_ptr<TemperatureStatistics> temperatureStatistics;
	const std::shared_ptr<std::ostream> output;
	const std::shared_ptr<CsvWriter> csvWriter;

	time_t lastUpdate;

	std::mutex mtx;
	Timer timer;
	std::deque<std::vector<std::string>> values;

	static std::string timeToString(std::time_t time);
	static std::string temperatureToString(float value);
	static std::shared_ptr<std::ostream> openFile(const std::string& fileName);

public:
	TemperaturePersister(
			const std::shared_ptr<TemperatureStatistics>& temperatureStatistics,
			const std::string& fileName,
			const std::shared_ptr<CsvWriterFactory>& csvWriterFactory
		);

	virtual ~TemperaturePersister();

	void periodicUpdate();
	void persistData();

	void startTimer();
	void stopTimer();
	virtual void onTimer() override;
};
