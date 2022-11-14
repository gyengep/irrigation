#include "TemperatureHistoryLogger.h"
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"
#include "Utils/CsvWriter.h"
#include "Utils/FixedDelaySchedulerRunnable.h"
#include "Utils/FunctionRunnable.h"
#include "Utils/ToString.h"
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;


TemperatureHistoryLogger::TemperatureHistoryLogger(
		const shared_ptr<TemperatureHistory>& temperatureHistory,
		const shared_ptr<CsvWriterFactory>& csvWriterFactory
	) :
	temperatureHistory(temperatureHistory),
	csvWriterFactory(csvWriterFactory)
{
}

TemperatureHistoryLogger::~TemperatureHistoryLogger() {
}

void TemperatureHistoryLogger::saveLog(const DateTime& from, const DateTime& to) {
	try {
		const auto statisticsValues = temperatureHistory->getTemperatureHistory(from, to);

		if (LOGGER.isLoggable(LogLevel::DEBUG)) {
			std::ostringstream oss;
			oss << LocalDateTime(from).toString() << "-" << LocalDateTime(to).toString() << ": ";
			oss << "[" << toCelsiusRange(statisticsValues.min, statisticsValues.max) << "]";
			LOGGER.debug("Saving temperature history: %s", oss.str().c_str());
		}

		const vector<string> historyTexts {
			LocalDateTime(from).toString("%Y.%m.%d %H:%M"),
			temperatureToString(statisticsValues.min),
			temperatureToString(statisticsValues.max),
			temperatureToString(statisticsValues.avg),
		};

		csvWriterFactory->create()->append(historyTexts);

	} catch (const exception& e) {
		LOGGER.debug("An error occured during write the temperature history file", e);
	}
}

void TemperatureHistoryLogger::start() {

	auto func = [this] {
		const LocalDateTime oneHourEarlier = LocalDateTime::now().addHours(-1);
		const LocalDateTime from(
				oneHourEarlier.getYears(), oneHourEarlier.getMonths(), oneHourEarlier.getDays(),
				oneHourEarlier.getHours(), 0, 0
			);

		const LocalDateTime to(
				oneHourEarlier.getYears(), oneHourEarlier.getMonths(), oneHourEarlier.getDays(),
				oneHourEarlier.getHours(), 59, 59
			);

		saveLog(from, to);
	};

	auto functionRunnbale = std::make_shared<FunctionRunnable>(func);
	auto everyHourSchedulerRunnable = std::make_shared<EveryHourSchedulerRunnable>(
			functionRunnbale,
			"TemperatureHistoryLogger"
		);

	workerThread = std::unique_ptr<Thread>(new Thread(
			everyHourSchedulerRunnable,
			"TemperatureHistoryLogger"
		));

	workerThread->start();
}

void TemperatureHistoryLogger::stop() {
	workerThread->stop();
	workerThread.reset();
}

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
