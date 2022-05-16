#include "TemperatureDependentSchedulerImpl.h"
#include "TemperatureToPercent.h"
#include "Logger/Logger.h"
#include "Utils/DateTime.h"
#include "Utils/TimeConversion.h"
#include "Utils/ToString.h"
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <limits>
#include <time.h>

using namespace std;

///////////////////////////////////////////////////////////////////////////////

TemperatureDependentSchedulerImpl::TemperatureDependentSchedulerImpl(const shared_ptr<TemperatureForecast>& temperatureForecast, const shared_ptr<TemperatureHistory>& temperatureHistory) :
	temperatureForecast(temperatureForecast),
	temperatureHistory(temperatureHistory),
	remainingPercent(0),
	requiredPercentForToday(0),
	lastRun(0),
	remainingCorrection(1.0f),
	minAdjustment(),
	maxAdjustment(),
	trim()
{
}

TemperatureDependentSchedulerImpl::TemperatureDependentSchedulerImpl(const TemperatureDependentSchedulerImpl& other) :
	TemperatureDependentSchedulerImpl(other.temperatureForecast,
			other.temperatureHistory,
			other.remainingCorrection,
			other.minAdjustment ? *other.minAdjustment : 0,
			other.maxAdjustment ? *other.maxAdjustment : 0,
			other.trim ? *other.trim : 0)
{
}

TemperatureDependentSchedulerImpl::TemperatureDependentSchedulerImpl(
		const shared_ptr<TemperatureForecast>& temperatureForecast,
		const shared_ptr<TemperatureHistory>& temperatureHistory,
		float remainingCorrection,
		unsigned minAdjustment, unsigned maxAdjustment,
		unsigned trim) :
	temperatureForecast(temperatureForecast),
	temperatureHistory(temperatureHistory),
	remainingPercent(0),
	requiredPercentForToday(0),
	lastRun(0),
	remainingCorrection(remainingCorrection)
{
	setMinAdjustment(minAdjustment);
	setMaxAdjustment(maxAdjustment);
	trimAdjustmentOver(trim);

}

TemperatureDependentSchedulerImpl::~TemperatureDependentSchedulerImpl() {
}

unsigned TemperatureDependentSchedulerImpl::getRequiredPercentForNextDay(const time_t now, float* temp) const {
	const DateTime dateTime(now);

	const float temperature = temperatureForecast->getTemperatureForecast(dateTime, dateTime.addSeconds(oneDayInSeconds - 1)).max;
	const unsigned result = TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(temperature);

	if (nullptr != temp) {
		*temp = temperature;
	}

	return result;
}

unsigned TemperatureDependentSchedulerImpl::getRequiredPercentForPreviousDay(const time_t now, float* temp) const {
	const DateTime dateTime(now);

	const float temperature = temperatureHistory->getTemperatureHistory(dateTime.addSeconds(-oneDayInSeconds), dateTime.addSeconds(-1)).max;
	const unsigned result = TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(temperature);

	if (nullptr != temp) {
		*temp = temperature;
	}

	return result;
}

enum TemperatureDependentSchedulerImpl::Day TemperatureDependentSchedulerImpl::getLastRunDay(const time_t rawtime) const {
	const unsigned currentDaysSinceEpoch = getElapsedDaysSinceEpoch(toLocalTime(rawtime));
	const unsigned lastRunDaySinceEpoch = getElapsedDaysSinceEpoch(toLocalTime(lastRun));

	if (currentDaysSinceEpoch == lastRunDaySinceEpoch) {
		return Day::TODAY;
	} else if (currentDaysSinceEpoch == (lastRunDaySinceEpoch + 1)) {
		return Day::YESTERDAY;
	} else {
		return Day::OTHER;
	}
}

std::string TemperatureDependentSchedulerImpl::dayToString(enum Day day) {
	switch (day) {
	case Day::TODAY:
		return "TODAY";
	case Day::YESTERDAY:
		return "YESTERDAY";
	case Day::OTHER:
		return "OTHER";
	default:
		throw std::logic_error("Invalid Day");
	}
}

///////////////////////////////////////////////////////////////////////////////

int TemperatureDependentSchedulerImpl::calculateRemainingPercentOther(const int remainingPercent, std::ostringstream& oss) const {
	oss << "\t\t" << std::setw(logIndentation) << "Calculate remainingPercent" << std::endl;

	const int result = 0;
	oss << "\t\t\t" << std::setw(logIndentation) << "result:" << toPercent(result) << std::endl;
	return result;
}

int TemperatureDependentSchedulerImpl::calculateRemainingPercentToday(const int remainingPercent, std::ostringstream& oss) const {
	oss << "\t\t" << std::setw(logIndentation) << "Calculate remainingPercent" << std::endl;
	oss << "\t\t\t" << std::setw(logIndentation) << "remainingPercent from previous run: " << toPercent(remainingPercent) << std::endl;

	const int result = remainingPercent;
	oss << "\t\t\t" << std::setw(logIndentation) << "result: " << toPercent(result) << std::endl;
	return result;
}

int TemperatureDependentSchedulerImpl::calculateRemainingPercentYesterday(const int remainingPercent, const std::time_t rawtime, std::ostringstream& oss) const {
	oss << "\t\t" << std::setw(logIndentation) << "Calculate remainingPercent" << std::endl;
	oss << "\t\t\t" << std::setw(logIndentation) << "remainingPercent from previous run: " << toPercent(remainingPercent) << std::endl;

	float temperature = 0.0f;
	unsigned yesterdayUsage = getRequiredPercentForPreviousDay(rawtime, &temperature);

	oss << "\t\t\t" << std::setw(logIndentation) << "yesterday usage: " << toPercent(yesterdayUsage) << " (" << toCelsius(temperature) << ")" << std::endl;

	if (nullptr != trim && yesterdayUsage > *trim) {
		yesterdayUsage = *trim;
	}

	int result = remainingPercent;
	result -= static_cast<int>(yesterdayUsage);
	result *= remainingCorrection;

	oss << "\t\t\t" << std::setw(logIndentation) << "yesterday usage (trim): " << toPercent(yesterdayUsage) << " (trim: " << optionalToString(trim) << ")" << std::endl;
	oss << "\t\t\t" << std::setw(logIndentation) << "remaining correction: " << remainingCorrection << std::endl;
	oss << "\t\t\t" << std::setw(logIndentation) << "result: " << toPercent(result) << std::endl;
	return result;
}

///////////////////////////////////////////////////////////////////////////////

unsigned TemperatureDependentSchedulerImpl::calculateRequiredPercentForToday(const int remainingPercent, const std::time_t rawtime, std::ostringstream& oss) const {
	oss << "\t\t" << std::setw(logIndentation) << "Calculate requiredPercentForToday" << std::endl;
	oss << "\t\t\t" << std::setw(logIndentation) << "remainingPercent: " << toPercent(remainingPercent) << std::endl;

	float temperature = 0.0f;
	unsigned forecastedUsage = getRequiredPercentForNextDay(rawtime, &temperature);

	oss << "\t\t\t" << std::setw(logIndentation) << "forecasted usage: " << toPercent(forecastedUsage) << " (" << toCelsius(temperature) << ")" << std::endl;

	if (nullptr != trim && forecastedUsage > *trim) {
		forecastedUsage = *trim;
	}


	int result = static_cast<int>(forecastedUsage);
	result -= remainingPercent;

	if (result < 0) {
		result = 0;
	}

	oss << "\t\t\t" << std::setw(logIndentation) << "forecasted usage (trim): " << toPercent(forecastedUsage) << " (trim: " << optionalToString(trim) << ")" << std::endl;
	oss << "\t\t\t" << std::setw(logIndentation) << "result: " << toPercent(result) << std::endl;
	return static_cast<unsigned>(result);
}

unsigned TemperatureDependentSchedulerImpl::calculateAdjustment(const unsigned requiredPercentForToday, std::ostringstream& oss) const {
	oss << "\t\t" << std::setw(logIndentation) << "Calculate adjustment" << std::endl;
	oss << "\t\t\t" << std::setw(logIndentation) << "requiredPercentForToday: " << toPercent(requiredPercentForToday) << std::endl;

	unsigned result = requiredPercentForToday;

	if (0 != result && nullptr != minAdjustment) {
		result = max(result, *minAdjustment);
	}
	oss << "\t\t\t" << std::setw(logIndentation) << "requiredPercent (min): " << toPercent(result) << " (min: " << optionalToString(minAdjustment) << ")" << std::endl;

	if (nullptr != maxAdjustment) {
		result = min(result, *maxAdjustment);
	}

	oss << "\t\t\t" << std::setw(logIndentation) << "requiredPercent (max): " << toPercent(result) << " (max: " << optionalToString(maxAdjustment) << ")" << std::endl;
	oss << "\t\t\t" << std::setw(logIndentation) << "result: " << toPercent(result) << std::endl;
	return result;
}

Scheduler::Result TemperatureDependentSchedulerImpl::process(const time_t rawtime) {

	if (nullptr == temperatureForecast) {
		throw logic_error("TemperatureDependentSchedulerImpl::process()  nullptr == temperatureForecast");
	}

	if (nullptr == temperatureHistory) {
		throw logic_error("TemperatureDependentSchedulerImpl::process()  nullptr == temperatureHistory");
	}

	const enum Day day = getLastRunDay(rawtime);
	lastRun = rawtime;

	std::ostringstream oss;
	oss.setf(std::ios::left, std::ios::adjustfield);

	oss << "TemperatureDependentSchedulerImpl::process()" << std::endl;
	oss << "\t" << std::setw(logIndentation) << "last run: " << dayToString(day) << " (" << toLocalTimeStr(lastRun, "%F %T") << ")" << std::endl;
	oss << "\t" << std::setw(logIndentation) << "remainingPercent from previous run: " << toPercent(remainingPercent) << std::endl;
	oss << "\t" << std::setw(logIndentation) << "requiredPercentForToday from previous run: " << toPercent(requiredPercentForToday) << std::endl;

	switch (day) {
	case Day::TODAY:
		remainingPercent = calculateRemainingPercentToday(remainingPercent, oss);
		break;
	case Day::YESTERDAY:
		remainingPercent = calculateRemainingPercentYesterday(remainingPercent, rawtime, oss);
		break;
	case Day::OTHER:
		remainingPercent = calculateRemainingPercentOther(remainingPercent, oss);
		break;
	default:
		throw std::logic_error("Invalid Day");
	}
	oss << "\t" << std::setw(logIndentation) << "remainingPercent: " << toPercent(remainingPercent) << std::endl;


	if (Day::YESTERDAY == day || Day::OTHER == day) {
		requiredPercentForToday = calculateRequiredPercentForToday(remainingPercent, rawtime, oss);
	}
	oss << "\t" << std::setw(logIndentation) << "requiredPercentForToday: " << toPercent(requiredPercentForToday) << std::endl;


	const unsigned adjustment = calculateAdjustment(requiredPercentForToday, oss);
	oss << "\t" << std::setw(logIndentation) << "adjustment: " << toPercent(adjustment) << std::endl;

	remainingPercent += adjustment;

	if (requiredPercentForToday > adjustment) {
		requiredPercentForToday -= adjustment;
	} else {
		requiredPercentForToday = 0;
	}

	oss << "\t" << std::setw(logIndentation) << "remainingPercent: " << toPercent(remainingPercent) << std::endl;
	oss << "\t" << std::setw(logIndentation) << "requiredPercentForToday: " << toPercent(requiredPercentForToday) << std::endl;

	LOGGER.trace(oss.str().c_str());

	return Scheduler::Result(adjustment);
}

nlohmann::json TemperatureDependentSchedulerImpl::saveTo() const {
	nlohmann::json j;
	j["remainingPercent"] = remainingPercent;
	j["lastRun"] = lastRun;
	return j;
}

void TemperatureDependentSchedulerImpl::loadFrom(const nlohmann::json& values) {
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

void TemperatureDependentSchedulerImpl::setRemainingCorrection(float a) {
	remainingCorrection = a;
}

void TemperatureDependentSchedulerImpl::setMinAdjustment(unsigned minAdjustment) {
	if (0 == minAdjustment) {
		this->minAdjustment.reset();
	} else {
		this->minAdjustment.reset(new unsigned(minAdjustment));
	}
}

void TemperatureDependentSchedulerImpl::setMaxAdjustment(unsigned maxAdjustment) {
	if (0 == maxAdjustment) {
		this->maxAdjustment.reset();
	} else {
		this->maxAdjustment.reset(new unsigned(maxAdjustment));
	}
}

void TemperatureDependentSchedulerImpl::trimAdjustmentOver(unsigned trim) {
	if (0 == trim) {
		this->trim.reset();
	} else {
		this->trim.reset(new unsigned(trim));
	}
}

TemperatureDependentSchedulerDTO TemperatureDependentSchedulerImpl::toTemperatureDependentSchedulerDto() const {
	return TemperatureDependentSchedulerDTO(
			remainingCorrection,
			minAdjustment ? *minAdjustment : 0, maxAdjustment ? *maxAdjustment : 0,
			trim ? *trim : 0);
}

void TemperatureDependentSchedulerImpl::updateFromTemperatureDependentSchedulerDto(const TemperatureDependentSchedulerDTO& schedulerDTO) {
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

std::string TemperatureDependentSchedulerImpl::optionalToString(const std::unique_ptr<unsigned>& value) {
	if (nullptr == value) {
		return "disabled";
	}

	return std::to_string(*value) + "%";
}

std::string TemperatureDependentSchedulerImpl::toString() const {
	std::ostringstream oss;
	oss << "TemperatureDependentScheduler{";
	oss << "remainingCorrection=" << remainingCorrection << ", ";
	oss << "minAdjustment=" << optionalToString(minAdjustment) << ", ";
	oss << "maxAdjustment=" << optionalToString(maxAdjustment) << ", ";
	oss << "trimOver=" << optionalToString(trim);
	oss << "}";
	return oss.str();
}
