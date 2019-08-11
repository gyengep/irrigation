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

const time_t TemperatureDependentScheduler::aDayInSeconds = chrono::duration_cast<chrono::seconds>(chrono::hours(24)).count();

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

	LOGGER.trace(">>> TemperatureDependentScheduler::process() <<<");

	struct tm timeinfo;

	const unsigned currentDaysSinceEpoch = getElapsedDaysSinceEpoch(*localtime_r(&rawtime, &timeinfo));
	const unsigned lastRunDaySinceEpoch = getElapsedDaysSinceEpoch(*localtime_r(&lastRun, &timeinfo));

	LOGGER.trace("%-30s%s", "current time", ctime(&rawtime));
	LOGGER.trace("%-30s%s", "last run", ctime(&lastRun));
	LOGGER.trace("%-30s%d%%", "remainingPercent", remainingPercent);

	lastRun = rawtime;

	if (currentDaysSinceEpoch == lastRunDaySinceEpoch) {

		LOGGER.trace("Last run is TODAY");

	} else {

		if (currentDaysSinceEpoch == (lastRunDaySinceEpoch + 1)) {
			LOGGER.trace("Last run is YESTERDAY");
			const int requiredPercentForPreviousDay = getRequiredPercentForPreviousDay(rawtime);
			LOGGER.trace("%-30s%d%%", "requiredPercentForPreviousDay", requiredPercentForPreviousDay);
			remainingPercent -= requiredPercentForPreviousDay;
			LOGGER.trace("%-30s%d%%", "remainingPercent", remainingPercent);
			remainingPercent *= remainingA;
			LOGGER.trace("%-30s%.1f", "remainingA", remainingA);
			LOGGER.trace("%-30s%d%%", "remainingPercent", remainingPercent);
		} else {
			LOGGER.trace("Last run is OTHER");
			remainingPercent = 0;
			LOGGER.trace("%-30s%d%%", "remainingPercent", remainingPercent);
		}

		requiredAdjustmentForWholeDay = (getRequiredPercentForNextDay(rawtime) - getRemainingPercent());
	}

	int adjustmentForThisScheduling = requiredAdjustmentForWholeDay;
	LOGGER.trace("%-30s%d%%", "adjustment", adjustmentForThisScheduling);

	if (adjustmentForThisScheduling < 0) {
		adjustmentForThisScheduling = 0;
	} else if (adjustmentForThisScheduling > 0) {
		adjustmentForThisScheduling = max(adjustmentForThisScheduling, minAdjustment);
		adjustmentForThisScheduling = min(adjustmentForThisScheduling, maxAdjustment);
	}

	requiredAdjustmentForWholeDay -= adjustmentForThisScheduling;

	LOGGER.trace("%-30s%d%%", "adjustment (min/max)", adjustmentForThisScheduling);

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
