#include "TemperatureHistoryLogger.h"
#include "TemperatureHistory.h"
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"
#include "Utils/CsvWriter.h"
#include "Utils/TimeConversion.h"
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;


TemperatureHistoryLogger::TemperatureHistoryLogger(
		const shared_ptr<TemperatureHistory>& temperatureHistory,
		const shared_ptr<CsvWriterFactory>& csvWriterFactory
	) :
	temperatureHistory(temperatureHistory),
	csvWriterFactory(csvWriterFactory),
	lastUpdateTime(time(nullptr))
{
/*
	if (csvWriter->stream()->tellp() == 0) {
		csvWriter->append(vector<string>{"Date", "MinTemperature", "MaxTemperature", "AvgTemperature"});
	}

	csvWriter->stream()->flush();
*/
}

TemperatureHistoryLogger::~TemperatureHistoryLogger() {
}

void TemperatureHistoryLogger::saveLog(const time_t& from, const time_t& to) {
	try {
		const auto statisticsValues = temperatureHistory->getTemperatureHistory(from, to);

		if (LOGGER.isLoggable(LogLevel::DEBUG)) {
			std::ostringstream oss;

			oss << toLocalTimeStr(from, "%F %T") << "-" << toLocalTimeStr(to, "%F %T") << ": ";
			oss << "[" << std::fixed << std::setw(2) << std::setprecision(1) << statisticsValues.min << "-";
			oss << std::fixed << std::setw(2) << std::setprecision(1) << statisticsValues.max << "]";

			LOGGER.debug("Saving temperature history: %s", oss.str().c_str());
		}

		const vector<string> historyTexts {
			toLocalTimeStr(from, "%Y.%m.%d %H:%M"),
			temperatureToString(statisticsValues.min),
			temperatureToString(statisticsValues.max),
			temperatureToString(statisticsValues.avg),
		};

		csvWriterFactory->create()->append(historyTexts);

	} catch (const exception& e) {
		LOGGER.debug("An error occured during write the temperature history file", e);
	}
}

void TemperatureHistoryLogger::run() {
	const auto now = time(nullptr);
	const auto period = getPreviousPeriod(now, std::chrono::hours(1));
	return saveLog(period.first, period.second);
}

/*
void TemperatureHistoryLogger::onTimer() {
	const auto currentTime = time(nullptr);
	const auto periodInSeconds = chrono::duration_cast<chrono::seconds>(period).count();

	if ((lastUpdateTime / periodInSeconds) != (currentTime / periodInSeconds)) {
		lastUpdateTime = currentTime;

		#ifdef ONTIMER_TRACE_LOG
		LOGGER.trace("TemperatureHistoryLogger::onTimer()");
		#endif

		const auto periodFromTo = getPreviousPeriod(currentTime, period);
		saveLog(periodFromTo.first, periodFromTo.second);
	} else {
		#ifdef ONTIMER_TRACE_LOG
		LOGGER.trace("TemperatureHistoryLogger::onTimer() SKIPPED");
		#endif
	}
}
*/
///////////////////////////////////////////////////////////////////////////////

string TemperatureHistoryLogger::temperatureToString(float value) {
	char buffer[32];
	sprintf(buffer, "%.1f", value);
	return string(buffer);
}

shared_ptr<ostream> TemperatureHistoryLogger::openFile(const string& fileName) {
	auto output = make_shared<ofstream>(fileName, ofstream::out | ofstream::app);

	if (output->fail()) {
		throw IOException(errno);
	}

	LOGGER.debug("Temperature history file: %s", fileName.c_str());
	return output;
}
