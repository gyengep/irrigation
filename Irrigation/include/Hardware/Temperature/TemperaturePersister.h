#pragma once
#include <ctime>
#include <deque>
#include <mutex>
#include <string>
#include <ostream>
#include <vector>
#include "Utils/Timer.h"

class CsvWriter;


class TemperaturePersister : public TimerCallback {
public:
	class CsvWriterFactory;

private:
	const std::shared_ptr<std::ostream> output;
	const std::shared_ptr<CsvWriter> csvWriter;

	std::mutex mtx;
	Timer timer;
	std::deque<std::vector<std::string>> values;

	static std::string timeToString(std::time_t time);
	static std::string temperatureToString(float value);

public:
	TemperaturePersister(const std::shared_ptr<std::ostream>& output, const std::shared_ptr<CsvWriterFactory>& csvWriterFactory);
	virtual ~TemperaturePersister();

	void append(std::time_t currentTime, float temperature);
	void appendInvalid(std::time_t currentTime);

	void persistData();

	void startTimer();
	void stopTimer();
	virtual void onTimer() override;

	static std::shared_ptr<TemperaturePersister> create(const std::string& fileName);
};


class TemperaturePersister::CsvWriterFactory {
public:
	virtual ~CsvWriterFactory() = default;
	virtual std::shared_ptr<CsvWriter> create(std::shared_ptr<std::ostream> output);
};
