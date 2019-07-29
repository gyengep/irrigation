#include "TemperatureDependentScheduler.h"
#include "Logger/Logger.h"
#include <algorithm>
#include <chrono>
#include <limits>

using namespace std;
using namespace TemperatureDependentScheduler;

///////////////////////////////////////////////////////////////////////////////

const time_t BaseScheduler::aDayInSeconds = chrono::duration_cast<chrono::seconds>(chrono::hours(24)).count();

BaseScheduler::BaseScheduler(const shared_ptr<TemperatureForecast>& temperatureForecast, const shared_ptr<TemperatureHistory>& temperatureHistory) :
	temperatureForecast(temperatureForecast),
	temperatureHistory(temperatureHistory),
	remainingPercent(0),
	maxAdjustment(numeric_limits<int>::max()),
	adjustment(0),
	first(true)
{
}

BaseScheduler::~BaseScheduler() {
}

void BaseScheduler::setTemperatureAndPercents(const vector<pair<float, int>>& temperatureAndPercents) {
	this->temperatureAndPercents = temperatureAndPercents;
}

int BaseScheduler::getRequiredPercentFromTemperature(float temperature) const {
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

int BaseScheduler::getRequiredPercentForNextDay(const time_t now) const {
	const float temperature = temperatureForecast->getForecastValues(now, now + aDayInSeconds).max;
	const int result = getRequiredPercentFromTemperature(temperature);

	LOGGER.trace("BaseScheduler: temperature forecast: %.1f°C, required adjustment: %d%%", temperature, result);
	return result;
}

int BaseScheduler::getRequiredPercentForPreviousDay(const time_t now) const {
	const float temperature = temperatureHistory->getHistoryValues(now - aDayInSeconds, now).max;
	const int result = getRequiredPercentFromTemperature(temperature);

	LOGGER.trace("BaseScheduler: measured temperature: %.1f°C, required adjustment: %d%%", temperature, result);
	return result;
}

bool BaseScheduler::isDayScheduled(const tm&) const {
	return (adjustment > 0);
}

unsigned BaseScheduler::getAdjustment() const {
	return adjustment;
}

void BaseScheduler::process(const tm& timeinfo) {
	LOGGER.trace(">>> BaseScheduler::process() <<<");
	LOGGER.trace("%-30s%d", "remainingPercent", remainingPercent);

	if (!first) {
		const int requiredPercentForPreviousDay = getRequiredPercentForPreviousDay();
		remainingPercent -= requiredPercentForPreviousDay;
		LOGGER.trace("%-30s%d", "requiredPercentForPreviousDay", requiredPercentForPreviousDay);
		LOGGER.trace("%-30s%d", "remainingPercent", remainingPercent);
	} else {
		first = false;
	}

	adjustment = calculateAdjustment();
	LOGGER.trace("%-30s%d", "adjustment", adjustment);

	adjustment = max(adjustment, 0);
	adjustment = min(adjustment, maxAdjustment);
	LOGGER.trace("%-30s%d", "adjustment (min/max)", adjustment);

	remainingPercent += adjustment;
	LOGGER.trace("%-30s%d", "remainingPercent", remainingPercent);
}

nlohmann::json BaseScheduler::saveTo() const {
	nlohmann::json j;
	j["remainingPercent"] = remainingPercent;
	return j;
}

void BaseScheduler::loadFrom(const nlohmann::json& values) {
	auto it = values.find("remainingPercent");
	if (values.end() != it) {
		remainingPercent = it.value();
	}
}

///////////////////////////////////////////////////////////////////////////////

FixedAmountScheduler::FixedAmountScheduler(const shared_ptr<TemperatureForecast>& temperatureForecast, const shared_ptr<TemperatureHistory>& temperatureHistory) :
	BaseScheduler(temperatureForecast, temperatureHistory)
{
}

FixedAmountScheduler::~FixedAmountScheduler() {
}

int FixedAmountScheduler::calculateAdjustment() {
	const int requiredPercentForNextDay = getRequiredPercentForNextDay();
	LOGGER.trace("%-30s%d", "requiredPercentForNextDay", requiredPercentForNextDay);

	if (requiredPercentForNextDay > getRemainingPercent()) {
		return 100;
	} else {
		return 0;
	}
}

///////////////////////////////////////////////////////////////////////////////

FixedPeriodScheduler::FixedPeriodScheduler(const shared_ptr<TemperatureForecast>& temperatureForecast, const shared_ptr<TemperatureHistory>& temperatureHistory) :
	BaseScheduler(temperatureForecast, temperatureHistory)
{
}

FixedPeriodScheduler::~FixedPeriodScheduler() {
}

int FixedPeriodScheduler::calculateAdjustment() {
	const int requiredPercentForNextDay = getRequiredPercentForNextDay();
	LOGGER.trace("%-30s%d", "requiredPercentForNextDay", requiredPercentForNextDay);

	return (requiredPercentForNextDay - getRemainingPercent());
}
