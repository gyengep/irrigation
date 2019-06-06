#include "TemperaturePersister.h"
#include "TemperatureStatistics.h"
#include "Utils/CsvWriterImpl.h"
#include "Logger/Logger.h"
#include "Exceptions/Exceptions.h"
#include <fstream>

using namespace std;


TemperaturePersister::TemperaturePersister(
		const chrono::duration<int64_t>& period,
		const shared_ptr<TemperatureStatistics>& temperatureStatistics,
		const string& fileName,
		const shared_ptr<CsvWriterFactory>& csvWriterFactory
	) :
	periodInSeconds(chrono::duration_cast<chrono::seconds>(period).count()),
	temperatureStatistics(temperatureStatistics),
	output(openFile(fileName)),
	csvWriter(csvWriterFactory->create(output)),
	lastUpdate(chrono::system_clock::to_time_t(chrono::system_clock::now()))
{
	LOGGER.trace("TemperatureHistory period: %lld seconds", periodInSeconds);

	if (output->tellp() == 0) {
		csvWriter->append(vector<string>{"Date", "MinTemperature", "MaxTemperature", "AvgTemperature"});
	}

	output->flush();
}

TemperaturePersister::~TemperaturePersister() {
}

void TemperaturePersister::periodicUpdate() {
	const time_t currentTime = chrono::system_clock::to_time_t(chrono::system_clock::now());

	if (lastUpdate / periodInSeconds != currentTime / periodInSeconds) {

		const time_t periodStart = ((currentTime / periodInSeconds) - 1) * periodInSeconds;
		const time_t periodEnd = (currentTime / periodInSeconds) * periodInSeconds - 1;

		if (LOGGER.isLoggable(LogLevel::TRACE)) {
			LOGGER.trace("TemperaturePersister::periodic()");
			LOGGER.trace("\tfrom: %s", timeToString(periodStart).c_str());
			LOGGER.trace("\tto: %s", timeToString(periodEnd).c_str());
		}

		try {
			const auto statisticsValues = temperatureStatistics->getStatistics(periodStart, periodEnd);
			const vector<string> statisticsTexts {
				timeToString(periodStart),
				temperatureToString(statisticsValues.minTemperature),
				temperatureToString(statisticsValues.maxTemperature),
				temperatureToString(statisticsValues.avgTemperature),
			};

			csvWriter->append(statisticsTexts);
			output->flush();

		} catch (const exception& e) {
			LOGGER.trace("An error occured during write the temperature history file", e);
		}

		lastUpdate = currentTime;
	}
}

///////////////////////////////////////////////////////////////////////////////

string TemperaturePersister::timeToString(time_t time) {
	char buffer[32];
	const tm* ptm = localtime(&time);
	strftime(buffer, 32, "%Y.%m.%d %H:%M", ptm);
	return string(buffer);
}

string TemperaturePersister::temperatureToString(float value) {
	char buffer[32];
	sprintf(buffer, "%.1f", value);
	return string(buffer);
}

shared_ptr<ostream> TemperaturePersister::openFile(const string& fileName) {
	auto output = make_shared<ofstream>(fileName, ofstream::out | ofstream::app);

	if (output->fail()) {
		throw IOException(errno);
	}

	LOGGER.debug("Temperature file: %s", fileName.c_str());
	return output;
}
