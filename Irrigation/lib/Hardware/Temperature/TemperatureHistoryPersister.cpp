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
	lastUpdateTime(time(nullptr))
{
	if (csvWriter->stream()->tellp() == 0) {
		csvWriter->append(vector<string>{"Date", "MinTemperature", "MaxTemperature", "AvgTemperature"});
	}

	csvWriter->stream()->flush();
}

TemperatureHistoryPersister::~TemperatureHistoryPersister() {
}

void TemperatureHistoryPersister::saveHistory(const time_t& from, const time_t& to) {
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

void TemperatureHistoryPersister::startTimer(const chrono::duration<int64_t>& period) {
	this->period = period;

	timer.reset(new Timer(chrono::minutes(1), Timer::ScheduleType::FIXED_DELAY, "TemperatureHistoryPersister"));
	timer->add(this);
	timer->start();
}

void TemperatureHistoryPersister::stopTimer() {
	timer->stop();
	timer.reset();
}

void TemperatureHistoryPersister::onTimer() {
	const auto currentTime = time(nullptr);
	const auto periodInSeconds = chrono::duration_cast<chrono::seconds>(period).count();

	if ((lastUpdateTime / periodInSeconds) != (currentTime / periodInSeconds)) {
		lastUpdateTime = currentTime;

		#ifdef ONTIMER_TRACE_LOG
		LOGGER.trace("TemperatureHistoryPersister::onTimer()");
		#endif

		const auto periodFromTo = getPreviousPeriod(currentTime, period);
		saveHistory(periodFromTo.first, periodFromTo.second);
	} else {
		#ifdef ONTIMER_TRACE_LOG
		LOGGER.trace("TemperatureHistoryPersister::onTimer() SKIPPED");
		#endif
	}
}

///////////////////////////////////////////////////////////////////////////////

string TemperatureHistoryPersister::timeToString(const time_t& rawTime) {
	char buffer[32];
	struct tm timeinfo;

	strftime(buffer, 32, "%Y.%m.%d %H:%M", localtime_r(&rawTime, &timeinfo));
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
