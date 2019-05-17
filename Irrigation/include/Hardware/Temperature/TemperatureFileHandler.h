#pragma once
#include <ctime>
#include <string>
#include <mutex>
#include <deque>
#include <vector>
#include "Utils/Timer.h"

class CsvWriter;


class TemperatureFileHandler : public TimerCallback {
	const std::string fileName;
	std::mutex mtx;
	Timer timer;
	std::deque<std::vector<std::string>> values;

	static std::string timeToString(std::time_t time);
	static std::string temperatureToString(float value);
	static std::string createFileName(const std::string& filePath);
	static void saveValues(const std::shared_ptr<CsvWriter>& csvWriter, std::deque<std::vector<std::string>> values);

public:
	TemperatureFileHandler(const std::string& filePath);
	virtual ~TemperatureFileHandler();

	void append(std::time_t currentTime, float temperature);
	void appendInvalid(std::time_t currentTime);

	void start();
	void stop();
	void saveToFile();

	virtual void onTimer() override;
};
