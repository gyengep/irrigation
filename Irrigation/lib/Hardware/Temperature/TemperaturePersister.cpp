#include "TemperaturePersister.h"
#include "TemperatureStatistics.h"
#include "Utils/CsvWriterImpl.h"
#include "Logger/Logger.h"
#include "Exceptions/Exceptions.h"
#include <fstream>

using namespace std;


TemperaturePersister::TemperaturePersister(
		const shared_ptr<TemperatureStatistics>& temperatureStatistics,
		const string& fileName,
		const shared_ptr<CsvWriterFactory>& csvWriterFactory
	) :
	temperatureStatistics(temperatureStatistics),
	output(openFile(fileName)),
	csvWriter(csvWriterFactory->create(output)),
	lastUpdate(chrono::system_clock::to_time_t(chrono::system_clock::now())),
	timer(*this, chrono::hours(1))
{
	if (output->tellp() == 0) {
		csvWriter->append(vector<string>{"Date", "MinTemperature", "MaxTemperature", "AvgTemperature"});
	}

	output->flush();
}

TemperaturePersister::~TemperaturePersister() {
	persistData();
}

void TemperaturePersister::periodicUpdate() {
	unique_lock<mutex> lock(mtx);

	time_t currentTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
	if (lastUpdate / 600 != currentTime / 600) {

		time_t periodStart = ((currentTime / 600) - 1) * 600;
		time_t periodEnd = (currentTime / 600) * 600 - 1;

		if (LOGGER.isLoggable(LogLevel::TRACE)) {
			struct tm * timeinfo;

			LOGGER.trace("TemperaturePersister::periodic()");

			timeinfo = localtime(&periodStart);
			LOGGER.trace("\tfrom: %s", asctime(timeinfo));

			timeinfo = localtime(&periodEnd);
			LOGGER.trace("\tto: %s", asctime(timeinfo));
		}

		try {
			auto statisticsValues = temperatureStatistics->getStatistics(periodStart, periodEnd);

			values.push_back(vector<string>{
				timeToString(periodStart),
				temperatureToString(statisticsValues.minTemperature),
				temperatureToString(statisticsValues.maxTemperature),
				temperatureToString(statisticsValues.avgTemperature),
			});
		} catch (const exception& e) {
			LOGGER.trace("An error occured", e);
		}

		lastUpdate = currentTime;
	}
}

void TemperaturePersister::persistData() {
	unique_lock<mutex> lock(mtx);
	if (!values.empty()) {
		deque<vector<string>> valuesCopy;
		valuesCopy.swap(values);
		lock.unlock();

		for (const auto& dateAndValue : valuesCopy) {
			csvWriter->append(dateAndValue);
		}

		output->flush();
		LOGGER.trace("TemperaturePersister::persistData()");
	}
}

///////////////////////////////////////////////////////////////////////////////

void TemperaturePersister::startTimer() {
	timer.start();
}

void TemperaturePersister::stopTimer() {
	timer.stop();
}

void TemperaturePersister::onTimer() {
	persistData();
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
