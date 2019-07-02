#include "TemperatureHistory.h"
#include "TemperatureStatistics.h"
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"
#include "Utils/CsvWriterImpl.h"
#include <fstream>

using namespace std;


TemperatureHistory::TemperatureHistory(
		const shared_ptr<TemperatureStatistics>& temperatureStatistics,
		const chrono::seconds& period,
		const string& fileName,
		const shared_ptr<CsvWriterFactory>& csvWriterFactory
	) : TemperatureHistory(
			temperatureStatistics,
			period,
			csvWriterFactory->create(openFile(fileName))
		)
{
}

TemperatureHistory::TemperatureHistory(
		const shared_ptr<TemperatureStatistics>& temperatureStatistics,
		const chrono::seconds& period,
		const std::shared_ptr<CsvWriter>& csvWriter
	) :
	periodInSeconds(chrono::duration_cast<chrono::seconds>(period).count()),
	temperatureStatistics(temperatureStatistics),
	csvWriter(csvWriter),
	lastUpdate(time(nullptr))
{
	if (csvWriter->stream()->tellp() == 0) {
		csvWriter->append(vector<string>{"Date", "MinTemperature", "MaxTemperature", "AvgTemperature"});
	}

	csvWriter->stream()->flush();
}

TemperatureHistory::~TemperatureHistory() {
}

void TemperatureHistory::saveHistory(const time_t from, const time_t to) {
	try {
		const auto statisticsValues = temperatureStatistics->getStatisticsValues(from, to);
		const vector<string> statisticsTexts {
			timeToString(from),
			temperatureToString(statisticsValues.min),
			temperatureToString(statisticsValues.max),
			temperatureToString(statisticsValues.avg),
		};

		csvWriter->append(statisticsTexts);
		csvWriter->stream()->flush();

	} catch (const exception& e) {
		LOGGER.debug("An error occured during write the temperature history file", e);
	}
}

void TemperatureHistory::startTimer() {
	timer.reset(new Timer(this, chrono::minutes(1), Timer::ScheduleType::FIXED_DELAY));
	timer->start();
}

void TemperatureHistory::stopTimer() {
	timer->stop();
	timer.reset();
}

void TemperatureHistory::onTimer() {
	const auto currentTime = time(nullptr);

	if ((lastUpdate / periodInSeconds) != (currentTime / periodInSeconds)) {
		lastUpdate = currentTime;

		LOGGER.trace("TemperatureHistory::onTimer()");

		const time_t from = ((currentTime / periodInSeconds) - 1) * periodInSeconds;
		const time_t to = (currentTime / periodInSeconds) * periodInSeconds - 1;
		saveHistory(from, to);
	} else {
		//LOGGER.trace("TemperatureHistory::onTimer() SKIPPED");
	}
}

///////////////////////////////////////////////////////////////////////////////

string TemperatureHistory::timeToString(time_t time) {
	char buffer[32];
	const tm* ptm = localtime(&time);
	strftime(buffer, 32, "%Y.%m.%d %H:%M", ptm);
	return string(buffer);
}

string TemperatureHistory::temperatureToString(float value) {
	char buffer[32];
	sprintf(buffer, "%.1f", value);
	return string(buffer);
}

shared_ptr<ostream> TemperatureHistory::openFile(const string& fileName) {
	auto output = make_shared<ofstream>(fileName, ofstream::out | ofstream::app);

	if (output->fail()) {
		throw IOException(errno);
	}

	LOGGER.debug("Temperature file: %s", fileName.c_str());
	return output;
}
