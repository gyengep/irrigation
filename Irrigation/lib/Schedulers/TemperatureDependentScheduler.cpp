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
	remainingCorrection(1.0f),
	minAdjustment(100),
	maxAdjustment(0),
	trim(0)
{
}

TemperatureDependentScheduler::TemperatureDependentScheduler(
		const shared_ptr<TemperatureForecast>& temperatureForecast,
		const shared_ptr<TemperatureHistory>& temperatureHistory,
		float remainingCorrection,
		int minAdjustment, int maxAdjustment,
		int trim) :
	temperatureForecast(temperatureForecast),
	temperatureHistory(temperatureHistory),
	requiredAdjustmentForWholeDay(0),
	remainingPercent(0),
	lastRun(0),
	remainingCorrection(remainingCorrection),
	minAdjustment(minAdjustment),
	maxAdjustment(maxAdjustment),
	trim(trim)
{
}

TemperatureDependentScheduler::~TemperatureDependentScheduler() {
}

int TemperatureDependentScheduler::getRequiredPercentForNextDay(const time_t now) const {
	const float temperature = temperatureForecast->getForecastValues(now, now + oneDayInSeconds - 1).max;
	const int result = TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(temperature);

	LOGGER.trace("TemperatureDependentScheduler temperature forecast:\n"
			"\tforecasted temperature:   %.1f°C\n"
			"\trequired adjustment:      %d%%",
			temperature,
			result
		);

	//LOGGER.trace("TemperatureDependentScheduler: temperature forecast: %.1f°C, required adjustment: %d%%", temperature, result);
	return result;
}

int TemperatureDependentScheduler::getRequiredPercentForPreviousDay(const time_t now) const {
	const float temperature = temperatureHistory->getHistoryValues(now - oneDayInSeconds, now - 1).max;
	const int result = TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(temperature);

	LOGGER.trace("TemperatureDependentScheduler temperature history:\n"
			"\tmeasured temperature:     %.1f°C\n"
			"\trequired adjustment:      %d%%",
			temperature,
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

	const unsigned currentDaysSinceEpoch = getElapsedDaysSinceEpoch(toLocalTime(rawtime));
	const unsigned lastRunDaySinceEpoch = getElapsedDaysSinceEpoch(toLocalTime(lastRun));

	if (LOGGER.isLoggable(LogLevel::TRACE)) {
		LOGGER.trace("%-30s%s", "current time", toLocalTimeStr(rawtime, "%F %T").c_str());
		LOGGER.trace("%-30s%s", "last run", toLocalTimeStr(lastRun, "%F %T").c_str());
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

			if (trim > 0 && requiredPercentForPreviousDay > trim) {
				requiredPercentForPreviousDay = trim;

				LOGGER.trace("%-30s", "TRIM requiredPercentForPreviousDay");
				LOGGER.trace("%-30s%d%%", "requiredPercentForPreviousDay", requiredPercentForPreviousDay);
			}

			remainingPercent -= requiredPercentForPreviousDay;
			LOGGER.trace("%-30s%d%%", "remainingPercent", remainingPercent);
			remainingPercent *= remainingCorrection;
			LOGGER.trace("%-30s%.1f", "remainingCorrection", remainingCorrection);
			LOGGER.trace("%-30s%d%%", "NEW remainingPercent", remainingPercent);

		} else {
			LOGGER.trace("Last run is OTHER");
			remainingPercent = 0;
			LOGGER.trace("%-30s%d%%", "NEW remainingPercent", remainingPercent);
		}

		int requiredPercentForNextDay = getRequiredPercentForNextDay(rawtime);

		LOGGER.trace("%-30s%d%%", "requiredPercentForNextDay", requiredPercentForNextDay);

		if (trim > 0 && requiredPercentForNextDay > trim) {
			requiredPercentForNextDay = trim;

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
		if (maxAdjustment > 0) {
			adjustmentForThisScheduling = min(adjustmentForThisScheduling, maxAdjustment);
		}
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
	remainingCorrection = a;
}

void TemperatureDependentScheduler::setMinAdjustment(unsigned minAdjustment) {
	this->minAdjustment = minAdjustment;
}

void TemperatureDependentScheduler::setMaxAdjustment(unsigned maxAdjustment) {
	this->maxAdjustment = maxAdjustment;
}

void TemperatureDependentScheduler::trimAdjustmentOver(unsigned trim) {
	this->trim = trim;
}

TemperatureDependentSchedulerDTO TemperatureDependentScheduler::toTemperatureDependentSchedulerDto() const {
	return TemperatureDependentSchedulerDTO(
			remainingCorrection,
			minAdjustment, maxAdjustment,
			trim);
}

void TemperatureDependentScheduler::updateFromTemperatureDependentSchedulerDto(const TemperatureDependentSchedulerDTO& schedulerDTO) {
	if (schedulerDTO.hasRemainingCorrection()) {
		setRemainingCorrection(schedulerDTO.getRemainingCorrection());
	}

	if (schedulerDTO.hasMinAdjustment()) {
		setMinAdjustment(schedulerDTO.getMinAdjustment());
	}

	if (schedulerDTO.hasMaxAdjustment()) {
		setMaxAdjustment(schedulerDTO.getMaxAdjustment());
	}

	if (schedulerDTO.hasTrim()) {
		trimAdjustmentOver(schedulerDTO.getTrim());
	}
}

string to_string(const TemperatureDependentScheduler& scheduler) {
	ostringstream oss;
	oss << scheduler;
	return oss.str();
}

ostream& operator<<(ostream& os, const TemperatureDependentScheduler& scheduler) {
	os << "TemperatureDependentScheduler{";
	os << "remainingCorrection=" << scheduler.remainingCorrection << ", ";
	os << "minAdjustment=" << scheduler.minAdjustment << ", ";
	os << "maxAdjustment=" << scheduler.maxAdjustment << ", ";
	os << "trimOver=" << scheduler.trim;
	os << "}";
	return os;
}
