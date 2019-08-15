#include "TemperatureDependentScheduler.h"
#include "TemperatureToPercent.h"
#include "Logger/Logger.h"
#include "Utils/TimeConversion.h"
#include <algorithm>
#include <chrono>
#include <limits>
#include <time.h>

using namespace std;

///////////////////////////////////////////////////////////////////////////////

TemperatureDependentScheduler::TemperatureDependentScheduler(const shared_ptr<TemperatureForecast>& temperatureForecast, const shared_ptr<TemperatureHistory>& temperatureHistory) :
	temperatureForecast(temperatureForecast),
	temperatureHistory(temperatureHistory),
	requiredAdjustmentForWholeDay(0),
	remainingPercent(0),
	lastRun(0),
	remainingA(1.0f),
	forecastA(1.0f),
	forecastB(0.0f),
	historyA(1.0f),
	historyB(0.0f),
	minAdjustment(0),
	maxAdjustment(numeric_limits<int>::max())
{
}

TemperatureDependentScheduler::~TemperatureDependentScheduler() {
}

int TemperatureDependentScheduler::getRequiredPercentForNextDay(const time_t now) const {
	const float temperature = temperatureForecast->getForecastValues(now, now + aDayInSeconds - 1).max;
	const float calculatedTemperature = forecastA * temperature + forecastB;
	const int result = TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(calculatedTemperature);

	LOGGER.trace("TemperatureDependentScheduler temperature forecast:\n"
			"\tforecasted temperature:   %.1f°C\n"
			"\ta, b:                     %.1f, %.1f\n"
			"\tcalculated temperature:   %.1f°C\n"
			"\trequired adjustment:      %d%%",
			temperature,
			forecastA, forecastB,
			calculatedTemperature,
			result
		);

	//LOGGER.trace("TemperatureDependentScheduler: temperature forecast: %.1f°C, required adjustment: %d%%", temperature, result);
	return result;
}

int TemperatureDependentScheduler::getRequiredPercentForPreviousDay(const time_t now) const {
	const float temperature = temperatureHistory->getHistoryValues(now - aDayInSeconds, now - 1).max;
	const float calculatedTemperature = historyA * temperature + historyB;
	const int result = TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(calculatedTemperature);

	LOGGER.trace("TemperatureDependentScheduler temperature history:\n"
			"\tmeasured temperature:     %.1f°C\n"
			"\ta, b:                     %.1f, %.1f\n"
			"\tcalculated value:         %.1f°C\n"
			"\trequired adjustment:      %d%%",
			temperature,
			historyA, historyB,
			calculatedTemperature,
			result
		);

//	LOGGER.trace("TemperatureDependentScheduler: measured temperature: %.1f°C, required adjustment: %d%%", temperature, result);
	return result;
}

Scheduler::Result TemperatureDependentScheduler::process(const time_t rawtime) {

	if (nullptr == temperatureForecast) {
		throw logic_error("TemperatureDependentScheduler::process()  nullptr == temperatureForecast");
	}

	if (nullptr == temperatureHistory) {
		throw logic_error("TemperatureDependentScheduler::process()  nullptr == temperatureHistory");
	}

	LOGGER.trace(">>>>>>>>>>> TemperatureDependentScheduler::process() <<<<<<<<<<<");

	struct tm timeinfo;

	const unsigned currentDaysSinceEpoch = getElapsedDaysSinceEpoch(*localtime_r(&rawtime, &timeinfo));
	const unsigned lastRunDaySinceEpoch = getElapsedDaysSinceEpoch(*localtime_r(&lastRun, &timeinfo));

	if (LOGGER.isLoggable(LogLevel::TRACE)) {
		char buffer[80];
		strftime(buffer, 80, "%F %T", localtime(&rawtime));
		LOGGER.trace("%-30s%s", "current time", buffer);

		strftime(buffer, 80, "%F %T", localtime(&lastRun));
		LOGGER.trace("%-30s%s", "last run", buffer);
	}

	LOGGER.trace("%-30s%d%%", "remainingPercent from previous run: ", remainingPercent);
	LOGGER.trace("%-30s", "CALCULATE REMAINING __BEGIN__");

	lastRun = rawtime;

	if (currentDaysSinceEpoch == lastRunDaySinceEpoch) {

		LOGGER.trace("Last run is TODAY");
		LOGGER.trace("%-30s%d%%", "NEW remainingPercent", remainingPercent);

	} else {

		if (currentDaysSinceEpoch == (lastRunDaySinceEpoch + 1)) {
			LOGGER.trace("Last run is YESTERDAY");
			int requiredPercentForPreviousDay = getRequiredPercentForPreviousDay(rawtime);

			LOGGER.trace("%-30s%d%%", "remainingPercent", remainingPercent);
			LOGGER.trace("%-30s%d%%", "requiredPercentForPreviousDay", requiredPercentForPreviousDay);

			if (trim != nullptr && requiredPercentForPreviousDay > * trim) {
				requiredPercentForPreviousDay = *trim;

				LOGGER.trace("%-30s", "TRIM requiredPercentForPreviousDay");
				LOGGER.trace("%-30s%d%%", "requiredPercentForPreviousDay", requiredPercentForPreviousDay);
			}

			remainingPercent -= requiredPercentForPreviousDay;
			LOGGER.trace("%-30s%d%%", "remainingPercent", remainingPercent);
			remainingPercent *= remainingA;
			LOGGER.trace("%-30s%.1f", "remainingA", remainingA);
			LOGGER.trace("%-30s%d%%", "NEW remainingPercent", remainingPercent);

		} else {
			LOGGER.trace("Last run is OTHER");
			remainingPercent = 0;
			LOGGER.trace("%-30s%d%%", "NEW remainingPercent", remainingPercent);
		}

		int requiredPercentForNextDay = getRequiredPercentForNextDay(rawtime);

		LOGGER.trace("%-30s%d%%", "requiredPercentForNextDay", requiredPercentForNextDay);

		if (trim != nullptr && requiredPercentForNextDay > *trim) {
			requiredPercentForNextDay = *trim;

			LOGGER.trace("%-30s", "TRIM requiredPercentForNextDay");
			LOGGER.trace("%-30s%d%%", "requiredPercentForNextDay", requiredPercentForNextDay);
		}

		requiredAdjustmentForWholeDay = requiredPercentForNextDay;
		requiredAdjustmentForWholeDay -= remainingPercent;

		LOGGER.trace("%-30s%d%%", "requiredAdjustmentForWholeDay", requiredAdjustmentForWholeDay);
	}

	LOGGER.trace("%-30s", "CALCULATE REMAINING __END__");

	int adjustmentForThisScheduling = requiredAdjustmentForWholeDay;

	LOGGER.trace("%-30s%d%%", "adjustment", adjustmentForThisScheduling);

	if (adjustmentForThisScheduling < 0) {
		adjustmentForThisScheduling = 0;
	} else if (adjustmentForThisScheduling > 0) {
		adjustmentForThisScheduling = max(adjustmentForThisScheduling, minAdjustment);
		adjustmentForThisScheduling = min(adjustmentForThisScheduling, maxAdjustment);
	}

	LOGGER.trace("%-30s%d%%", "adjustment (min/max)", adjustmentForThisScheduling);

	requiredAdjustmentForWholeDay -= adjustmentForThisScheduling;
	remainingPercent += adjustmentForThisScheduling;

	LOGGER.trace("%-30s%d%%", "remainingPercent", remainingPercent);

	return Scheduler::Result(static_cast<unsigned>(adjustmentForThisScheduling));
}

nlohmann::json TemperatureDependentScheduler::saveTo() const {
	nlohmann::json j;
	j["remainingPercent"] = remainingPercent;
	j["lastRun"] = lastRun;
	return j;
}

void TemperatureDependentScheduler::loadFrom(const nlohmann::json& values) {
	{
		auto it = values.find("remainingPercent");
		if (values.end() != it) {
			remainingPercent = it.value();
		}
	}

	{
		auto it = values.find("lastRun");
		if (values.end() != it) {
			lastRun = it.value();
		}
	}
}

void TemperatureDependentScheduler::setRemainingCorrection(float a) {
	remainingA = a;
}

void TemperatureDependentScheduler::setHistoryCorrection(float a, float b) {
	historyA = a;
	historyB = b;
}

void TemperatureDependentScheduler::setForecastCorrection(float a, float b) {
	forecastA = a;
	forecastB = b;
}

void TemperatureDependentScheduler::setMinAdjustment(unsigned minAdjustment) {
	this->minAdjustment = minAdjustment;
}

void TemperatureDependentScheduler::setMaxAdjustment(unsigned maxAdjustment) {
	this->maxAdjustment = maxAdjustment;
}

void TemperatureDependentScheduler::trimAdjustmentOver(unsigned percent) {
	trim.reset(new int(percent));
}

string to_string(const TemperatureDependentScheduler& scheduler) {
	ostringstream oss;
	oss << scheduler;
	return oss.str();
}

ostream& operator<<(ostream& os, const TemperatureDependentScheduler& scheduler) {
	os << "TemperatureDependentScheduler{";
	os << "minAdjustment=" << scheduler.minAdjustment << ", ";
	os << "maxAdjustment=" << scheduler.maxAdjustment << ", ";
	if (nullptr == scheduler.trim) {
		os << "trimOver=" << "disabled" << ", ";
	} else {
		os << "trimOver=" << *scheduler.trim << ", ";
	}
	os << "forecastA=" << scheduler.forecastA << ", forecastB=" << scheduler.forecastB << ", ";
	os << "historyA=" << scheduler.historyA << ", historyB=" << scheduler.historyB;
	os << "}";
	return os;
}
