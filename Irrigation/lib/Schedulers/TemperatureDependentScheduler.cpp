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
	adjustment(0)
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
	if (requiredPercentForNextDay == nullptr) {
		const float temperature = temperatureForecast->getForecastValues(now, now + aDayInSeconds).max;
		requiredPercentForNextDay.reset(new int(getRequiredPercentFromTemperature(temperature)));

		LOGGER.trace("BaseScheduler: temperature forecast: %.1f°C, required adjustment: %d%%", temperature, *requiredPercentForNextDay);
	}

	return *requiredPercentForNextDay;
}

int BaseScheduler::getRequiredPercentForPreviousDay(const time_t now) const {
	if (requiredPercentForPreviousDay.get() == nullptr) {
		const float temperature = temperatureHistory->getHistoryValues(now - aDayInSeconds, now).max;
		requiredPercentForPreviousDay.reset(new int(getRequiredPercentFromTemperature(temperature)));

		LOGGER.trace("BaseScheduler: measured temperature: %.1f°C, required adjustment: %d%%", temperature, *requiredPercentForPreviousDay);
	}

	return *requiredPercentForPreviousDay;
}

int BaseScheduler::getAndStoreRequiredPercent(int percentToStore) {
	const auto temporary = move(storedPercent);

	storedPercent.reset(new int(percentToStore));

	if (temporary == nullptr) {
		throw exception();
	}

	return *temporary;
}

bool BaseScheduler::isDayScheduled(const tm&) const {
	return (adjustment > 0);
}

unsigned BaseScheduler::getAdjustment() const {
	return adjustment;
}

nlohmann::json BaseScheduler::saveTo() const {
	nlohmann::json j;

	j["remainingPercent"] = remainingPercent;

	if (storedPercent != nullptr) {
		j["storedPercent"] = *storedPercent;
	}

	return j;
}

void BaseScheduler::loadFrom(const nlohmann::json& values) {
	{
		auto it = values.find("storedPercent");
		if (values.end() != it) {
			storedPercent.reset(new int(it.value()));
		}
	}

	{
		auto it = values.find("remainingPercent");
		if (values.end() != it) {
			remainingPercent = it.value();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

FixedAmountScheduler::FixedAmountScheduler(const shared_ptr<TemperatureForecast>& temperatureForecast, const shared_ptr<TemperatureHistory>& temperatureHistory) :
	BaseScheduler(temperatureForecast, temperatureHistory)
{
}

FixedAmountScheduler::~FixedAmountScheduler() {
}

void FixedAmountScheduler::process(const tm& timeinfo) {
	/*
	LOGGER.trace("FixedAmountScheduler::calculatePercent()");

	const int remainingPercent = getRemainingPercent();
	const int requiredPercentForNextDay = getRequiredPercentForNextDay();
	int usedPercentForPreviousDay = 0;
	int requiredPercentForPreviousDay = 0;

	try {
		requiredPercentForPreviousDay = getRequiredPercentForPreviousDay();
		usedPercentForPreviousDay = getAndStoreRequiredPercent(requiredPercentForNextDay);
	} catch (const exception&) {
	}

	LOGGER.trace("%-30s%d", "remainingPercent", remainingPercent);
	LOGGER.trace("%-30s%d", "requiredPercentForNextDay", requiredPercentForNextDay);
	LOGGER.trace("%-30s%d", "requiredPercentForPreviousDay", requiredPercentForPreviousDay);
	LOGGER.trace("%-30s%d", "usedPercentForPreviousDay", usedPercentForPreviousDay);

	int accumulatedPercent = remainingPercent;
	accumulatedPercent += requiredPercentForNextDay;
	accumulatedPercent += (requiredPercentForPreviousDay - usedPercentForPreviousDay);

	if (accumulatedPercent > remainingPercent) {
		return 100;
	} else {
		return 0;
	}
	*/
}

///////////////////////////////////////////////////////////////////////////////

FixedPeriodScheduler::FixedPeriodScheduler(const shared_ptr<TemperatureForecast>& temperatureForecast, const shared_ptr<TemperatureHistory>& temperatureHistory) :
	BaseScheduler(temperatureForecast, temperatureHistory)
{
}

FixedPeriodScheduler::~FixedPeriodScheduler() {
}

void FixedPeriodScheduler::process(const tm& timeinfo) {
	LOGGER.trace(">>> FixedPeriodScheduler::process() <<<");

	requiredPercentForNextDay.reset();
	requiredPercentForPreviousDay.reset();

	const int requiredPercentForNextDay = getRequiredPercentForNextDay();
	int usedPercentForPreviousDay = 0;
	int requiredPercentForPreviousDay = 0;

	try {
		usedPercentForPreviousDay = getAndStoreRequiredPercent(requiredPercentForNextDay);
		requiredPercentForPreviousDay = getRequiredPercentForPreviousDay();
	} catch (const exception&) {
		LOGGER.trace("Can not read stored percent");
	}

	LOGGER.trace("%-30s%d", "remainingPercent", remainingPercent);
	LOGGER.trace("%-30s%d", "requiredPercentForNextDay", requiredPercentForNextDay);
	LOGGER.trace("%-30s%d", "requiredPercentForPreviousDay", requiredPercentForPreviousDay);
	LOGGER.trace("%-30s%d", "usedPercentForPreviousDay", usedPercentForPreviousDay);

	adjustment = 0;
	adjustment += requiredPercentForNextDay;
	adjustment += requiredPercentForPreviousDay;
	adjustment -= usedPercentForPreviousDay;
	LOGGER.trace("%-30s%d", "adjustment", adjustment);

	adjustment = max(adjustment, 0);
	adjustment = min(adjustment, maxAdjustment);
	LOGGER.trace("%-30s%d", "adjustment (min/max)", adjustment);

	remainingPercent += adjustment;
	remainingPercent -= requiredPercentForPreviousDay;
}
