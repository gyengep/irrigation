#include "TemperatureHistoryPersister.h"
#include "TemperatureHistory.h"
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"
#include "Utils/CsvWriterImpl.h"
#include "Utils/TimePeriod.h"
#include <fstream>

using namespace std;


TemperatureHistoryPersister::TemperatureHistoryPersister(
		const shared_ptr<TemperatureHistory>& temperatureHistory,
		const shared_ptr<CsvWriterFactory>& csvWriterFactory,
		const string& fileName
	) : TemperatureHistoryPersister(
			temperatureHistory,
			csvWriterFactory->create(openFile(fileName))
		)
{
}

TemperatureHistoryPersister::TemperatureHistoryPersister(
		const shared_ptr<TemperatureHistory>& temperatureHistory,
		const shared_ptr<CsvWriter>& csvWriter
	) :
	temperatureHistory(temperatureHistory),
	csvWriter(csvWriter),
	lastUpdateTime(chrono::system_clock::now())
{
	if (csvWriter->stream()->tellp() == 0) {
		csvWriter->append(vector<string>{"Date", "MinTemperature", "MaxTemperature", "AvgTemperature"});
	}

	csvWriter->stream()->flush();
}

TemperatureHistoryPersister::~TemperatureHistoryPersister() {
}

void TemperatureHistoryPersister::saveHistory(const chrono::system_clock::time_point& from, const chrono::system_clock::time_point& to) {
	try {
		const auto statisticsValues = temperatureHistory->getHistoryValues(from, to);

		const vector<string> historyTexts {
			timeToString(from),
			temperatureToString(statisticsValues.min),
			temperatureToString(statisticsValues.max),
			temperatureToString(statisticsValues.avg),
		};

		csvWriter->append(historyTexts);
		csvWriter->stream()->flush();

	} catch (const exception& e) {
		LOGGER.debug("An error occured during write the temperature history file", e);
	}
}

void TemperatureHistoryPersister::startTimer(const std::chrono::duration<int64_t>& period) {
	this->period = period;

	timer.reset(new Timer(this, chrono::minutes(1), Timer::ScheduleType::FIXED_DELAY));
	timer->start();
}

void TemperatureHistoryPersister::stopTimer() {
	timer->stop();
	timer.reset();
}

void TemperatureHistoryPersister::onTimer() {
	const auto currentTime = chrono::system_clock::now();
	const auto currentTimeInSeconds = chrono::system_clock::to_time_t(currentTime);
	const auto lastUpdateTimeInSeconds = chrono::system_clock::to_time_t(lastUpdateTime);
	const auto periodInSeconds = chrono::duration_cast<chrono::seconds>(period).count();

	if ((lastUpdateTimeInSeconds / periodInSeconds) != (currentTimeInSeconds / periodInSeconds)) {
		lastUpdateTime = currentTime;
		LOGGER.trace("TemperatureHistoryPersister::onTimer()");

		const auto periodFromTo = getPreviousPeriod(chrono::system_clock::now(), period);
		saveHistory(periodFromTo.first, periodFromTo.second);
	} else {
		LOGGER.trace("TemperatureHistoryPersister::onTimer() SKIPPED");
	}
}

///////////////////////////////////////////////////////////////////////////////

string TemperatureHistoryPersister::timeToString(const std::chrono::system_clock::time_point& time) {
	const time_t rawTime = chrono::system_clock::to_time_t(time);
	const tm* ptm = localtime(&rawTime);
	char buffer[32];

	strftime(buffer, 32, "%Y.%m.%d %H:%M", ptm);
	return string(buffer);
}

string TemperatureHistoryPersister::temperatureToString(float value) {
	char buffer[32];
	sprintf(buffer, "%.1f", value);
	return string(buffer);
}

shared_ptr<ostream> TemperatureHistoryPersister::openFile(const string& fileName) {
	auto output = make_shared<ofstream>(fileName, ofstream::out | ofstream::app);

	if (output->fail()) {
		throw IOException(errno);
	}

	LOGGER.debug("Temperature history file: %s", fileName.c_str());
	return output;
}
