#include "TemperatureHistory.h"
#include "TemperatureStatistics.h"
#include "Utils/CsvWriterImpl.h"
#include "Logger/Logger.h"
#include "Exceptions/Exceptions.h"
#include <fstream>

using namespace std;


TemperatureHistory::TemperatureHistory(
		const chrono::duration<int64_t>& period,
		const shared_ptr<TemperatureStatistics>& temperatureStatistics,
		const string& fileName,
		const shared_ptr<CsvWriterFactory>& csvWriterFactory
	) : TemperatureHistory(
			period,
			temperatureStatistics,
			csvWriterFactory->create(openFile(fileName)),
			chrono::system_clock::to_time_t(chrono::system_clock::now())
		)
{
}

TemperatureHistory::TemperatureHistory(
		const chrono::duration<int64_t>& period,
		const shared_ptr<TemperatureStatistics>& temperatureStatistics,
		const std::shared_ptr<CsvWriter>& csvWriter,
		const std::time_t currentTime
	) :
	periodInSeconds(chrono::duration_cast<chrono::seconds>(period).count()),
	temperatureStatistics(temperatureStatistics),
	csvWriter(csvWriter),
	lastUpdate(currentTime)
{
	if (csvWriter->stream()->tellp() == 0) {
		csvWriter->append(vector<string>{"Date", "MinTemperature", "MaxTemperature", "AvgTemperature"});
	}

	csvWriter->stream()->flush();
}

TemperatureHistory::~TemperatureHistory() {
}

void TemperatureHistory::periodicUpdate() {
	const time_t currentTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
	periodicUpdate(currentTime);
}

void TemperatureHistory::periodicUpdate(std::time_t currentTime) {
	if ((lastUpdate / periodInSeconds) != (currentTime / periodInSeconds)) {

		const time_t periodStart = ((currentTime / periodInSeconds) - 1) * periodInSeconds;
		const time_t periodEnd = (currentTime / periodInSeconds) * periodInSeconds - 1;

		try {
			const auto statisticsValues = temperatureStatistics->getStatistics(periodStart, periodEnd);
			const vector<string> statisticsTexts {
				timeToString(periodStart),
				temperatureToString(statisticsValues.minTemperature),
				temperatureToString(statisticsValues.maxTemperature),
				temperatureToString(statisticsValues.avgTemperature),
			};

			csvWriter->append(statisticsTexts);
			csvWriter->stream()->flush();

		} catch (const exception& e) {
			LOGGER.debug("An error occured during write the temperature history file", e);
		}

		lastUpdate = currentTime;
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