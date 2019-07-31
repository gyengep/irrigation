#include "TemperatureDependentScheduler.h"
#include "Logger/Logger.h"
#include "Utils/TimePeriod.h"
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
	remainingPercent(0),
	adjustment(0),
	lastRun(0)
{
	temperatureAndPercents = vector<pair<float, int>>{
		{ 15.0f, 25 },
		{ 25.0f, 50 },
		{ 35.0f, 100 }
	};

}

TemperatureDependentScheduler::~TemperatureDependentScheduler() {
}

void TemperatureDependentScheduler::setTemperatureAndPercents(const vector<pair<float, int>>& temperatureAndPercents) {
	this->temperatureAndPercents = temperatureAndPercents;
}

int TemperatureDependentScheduler::getRequiredPercentFromTemperature(float temperature) const {
	if (temperature < temperatureAndPercents.front().first) {
		return 0;
	}

	for (auto it = next(temperatureAndPercents.begin()); it != temperatureAndPercents.end(); ++it) {
		if (prev(it)->first <= temperature && temperature < it->first) {
			const double t0 = prev(it)->first;
			const double t1 = it->first;
			const double p0 = prev(it)->second;
			const double p1 = it->second;
			const double ratio = (temperature - t0) / (t1 - t0);
			return ((p1 - p0) * ratio ) + p0;
		}
	}

	return temperatureAndPercents.back().second;
}

int TemperatureDependentScheduler::getRequiredPercentForNextDay(const time_t now) const {
	LOGGER.trace("getForecastValues(%llu, %llu)", (long long unsigned)now, (long long unsigned)now + aDayInSeconds - 1);
	const float temperature = temperatureForecast->getForecastValues(now, now + aDayInSeconds - 1).max;
	const int result = getRequiredPercentFromTemperature(temperature);

	LOGGER.trace("TemperatureDependentScheduler: temperature forecast: %.1f�C, required adjustment: %d%%", temperature, result);
	return result;
}

int TemperatureDependentScheduler::getRequiredPercentForPreviousDay(const time_t now) const {
	LOGGER.trace("getHistoryValues(%llu, %llu)", (long long unsigned)now - aDayInSeconds, (long long unsigned)now - 1);
	const float temperature = temperatureHistory->getHistoryValues(now - aDayInSeconds, now - 1).max;
	const int result = getRequiredPercentFromTemperature(temperature);

	LOGGER.trace("TemperatureDependentScheduler: measured temperature: %.1f�C, required adjustment: %d%%", temperature, result);
	return result;
}

int TemperatureDependentScheduler::calculateAdjustment(const time_t) {
	throw logic_error("Method not implemented: TemperatureDependentScheduler::calculateAdjustment()");
}

bool TemperatureDependentScheduler::isDayScheduled(const tm&) const {
	return (adjustment > 0);
}

unsigned TemperatureDependentScheduler::getAdjustment() const {
	return adjustment;
}

void TemperatureDependentScheduler::process(const tm& timeinfo) {
	if (nullptr == temperatureForecast) {
		throw logic_error("TemperatureDependentScheduler::process()  nullptr == temperatureForecast");
	}

	if (nullptr == temperatureHistory) {
		throw logic_error("TemperatureDependentScheduler::process()  nullptr == temperatureHistory");
	}

	LOGGER.trace(">>> TemperatureDependentScheduler::process() <<<");
	LOGGER.trace("%-30s%d", "remainingPercent", remainingPercent);

	tm timeinfoCopy {0};
	memcpy(&timeinfoCopy, &timeinfo, sizeof(timeinfo));

	const time_t currentTime = timelocal(&timeinfoCopy);
	const auto yesterday = getPreviousPeriod(currentTime, chrono::hours(24));
	const auto today = getCurrentPeriod(currentTime, chrono::hours(24));

	const auto savedLastRun = lastRun;
	lastRun = currentTime;

	LOGGER.trace("Yesterday start: %s", asctime(localtime(&yesterday.first)));
	LOGGER.trace("Yesterday end:   %s", asctime(localtime(&yesterday.second)));
	LOGGER.trace("Today start:     %s", asctime(localtime(&today.first)));
	LOGGER.trace("Today end:       %s", asctime(localtime(&today.second)));
	LOGGER.trace("Last run:        %s", asctime(localtime(&savedLastRun)));
	LOGGER.trace("Current time:    %s", asctime(localtime(&currentTime)));

	if (today.first <= savedLastRun &&  savedLastRun <= today.second) {
		LOGGER.trace("Last run is TODAY");
		adjustment = 0;
		return;
	}

	if (yesterday.first <= savedLastRun &&  savedLastRun <= yesterday.second) {
		LOGGER.trace("Last run is YESTERDAY");
		const int requiredPercentForPreviousDay = getRequiredPercentForPreviousDay(currentTime);
		remainingPercent -= requiredPercentForPreviousDay;
		LOGGER.trace("%-30s%d", "requiredPercentForPreviousDay", requiredPercentForPreviousDay);
	} else {
		LOGGER.trace("Last run is OTHER");
		remainingPercent = 0;
	}

	LOGGER.trace("%-30s%d", "remainingPercent", remainingPercent);

	adjustment = calculateAdjustment(currentTime);
	LOGGER.trace("%-30s%d", "adjustment", adjustment);

	adjustment = max(adjustment, 0);
	LOGGER.trace("%-30s%d", "adjustment (min/max)", adjustment);

	remainingPercent += adjustment;
	LOGGER.trace("%-30s%d", "remainingPercent", remainingPercent);

}

nlohmann::json TemperatureDependentScheduler::saveTo() const {
	LOGGER.trace("TemperatureDependentScheduler::saveTo()");

	nlohmann::json j;
	j["remainingPercent"] = remainingPercent;
	j["lastRun"] = lastRun;

	LOGGER.trace("remainingPercent: %d", remainingPercent);
	LOGGER.trace("lastRun: %llu", (long long unsigned)lastRun);
	return j;
}

void TemperatureDependentScheduler::loadFrom(const nlohmann::json& values) {
	LOGGER.trace("TemperatureDependentScheduler::loadFrom()");

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

	LOGGER.trace("remainingPercent: %d", remainingPercent);
	LOGGER.trace("lastRun: %llu", (long long unsigned)lastRun);
}

///////////////////////////////////////////////////////////////////////////////

FixedAmountScheduler::FixedAmountScheduler(const shared_ptr<TemperatureForecast>& temperatureForecast, const shared_ptr<TemperatureHistory>& temperatureHistory) :
	TemperatureDependentScheduler(temperatureForecast, temperatureHistory)
{
}

FixedAmountScheduler::~FixedAmountScheduler() {
}

int FixedAmountScheduler::calculateAdjustment(const time_t rawTime) {
	const int requiredPercentForNextDay = getRequiredPercentForNextDay(rawTime);
	LOGGER.trace("%-30s%d", "requiredPercentForNextDay", requiredPercentForNextDay);

	if (requiredPercentForNextDay > getRemainingPercent()) {
		return 100;
	} else {
		return 0;
	}
}

///////////////////////////////////////////////////////////////////////////////

FixedPeriodScheduler::FixedPeriodScheduler(const shared_ptr<TemperatureForecast>& temperatureForecast, const shared_ptr<TemperatureHistory>& temperatureHistory) :
	TemperatureDependentScheduler(temperatureForecast, temperatureHistory)
{
}

FixedPeriodScheduler::~FixedPeriodScheduler() {
}

int FixedPeriodScheduler::calculateAdjustment(const time_t rawTime) {
	const int requiredPercentForNextDay = getRequiredPercentForNextDay(rawTime);
	LOGGER.trace("%-30s%d", "requiredPercentForNextDay", requiredPercentForNextDay);

	return (requiredPercentForNextDay - getRemainingPercent());
}
