#include "Impl/TemperatureDependentSchedulerImpl.h"
#include "TemperatureToPercent.h"
#include "Logger/Logger.h"
#include "Utils/DateTime.h"
#include "Utils/ToString.h"
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <limits>
#include <sstream>

///////////////////////////////////////////////////////////////////////////////

TemperatureDependentSchedulerImplFactory::TemperatureDependentSchedulerImplFactory(const std::shared_ptr<TemperatureForecast>& temperatureForecast, const std::shared_ptr<TemperatureHistory>& temperatureHistory) :
	temperatureForecast(temperatureForecast),
	temperatureHistory(temperatureHistory)
{
}

TemperatureDependentSchedulerPtr TemperatureDependentSchedulerImplFactory::create() const {
	return std::make_shared<TemperatureDependentSchedulerImpl>(
			temperatureForecast,
			temperatureHistory
		);
}

///////////////////////////////////////////////////////////////////////////////

TemperatureDependentSchedulerImpl::TemperatureDependentSchedulerImpl(
	const std::shared_ptr<TemperatureForecast>& temperatureForecast,
	const std::shared_ptr<TemperatureHistory>& temperatureHistory
) :
	temperatureForecast(temperatureForecast),
	temperatureHistory(temperatureHistory),
	remainingPercent(0),
	requiredPercentForToday(0),
	lastRun(DateTime::epoch()),
	remainingCorrection(1.0f),
	minAdjustment(),
	maxAdjustment()
{
}

TemperatureDependentSchedulerImpl::TemperatureDependentSchedulerImpl(
	const std::shared_ptr<TemperatureForecast>& temperatureForecast,
	const std::shared_ptr<TemperatureHistory>& temperatureHistory,
	float remainingCorrection,
	unsigned minAdjustment, unsigned maxAdjustment
) :
	temperatureForecast(temperatureForecast),
	temperatureHistory(temperatureHistory),
	remainingPercent(0),
	requiredPercentForToday(0),
	lastRun(DateTime::epoch()),
	remainingCorrection(remainingCorrection)
{
	setMinAdjustment(minAdjustment);
	setMaxAdjustment(maxAdjustment);
}

TemperatureDependentSchedulerImpl::~TemperatureDependentSchedulerImpl() {
}

unsigned TemperatureDependentSchedulerImpl::getRequiredPercentForNextDay(const DateTime& dateTime, float* temp) const {
	const float temperature = temperatureForecast->getTemperatureForecast(dateTime, dateTime + std::chrono::hours(24) - std::chrono::seconds(1)).max;
	const unsigned result = TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(temperature);

	if (nullptr != temp) {
		*temp = temperature;
	}

	return result;
}

unsigned TemperatureDependentSchedulerImpl::getRequiredPercentForPreviousDay(const DateTime& dateTime, float* temp) const {
	const float temperature = temperatureHistory->getTemperatureHistory(dateTime - std::chrono::hours(24), dateTime - std::chrono::seconds(1)).max;
	const unsigned result = TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(temperature);

	if (nullptr != temp) {
		*temp = temperature;
	}

	return result;
}

enum TemperatureDependentSchedulerImpl::Day TemperatureDependentSchedulerImpl::getLastRunDay(const LocalDateTime& localDateTime) const {

	if (localDateTime.getYears() == lastRun.getYears() &&
		localDateTime.getMonths() == lastRun.getMonths() &&
		localDateTime.getDays() == lastRun.getDays())
	{
		return Day::TODAY;
	}

	LocalDateTime oneDayEarlier = localDateTime.addDays(-1);

	if (oneDayEarlier.getYears() == lastRun.getYears() &&
		oneDayEarlier.getMonths() == lastRun.getMonths() &&
		oneDayEarlier.getDays() == lastRun.getDays())
	{
		return Day::YESTERDAY;
	}

	return Day::OTHER;
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

int TemperatureDependentSchedulerImpl::calculateRemainingPercentYesterday(const int remainingPercent, const DateTime& dateTime, std::ostringstream& oss) const {
	oss << "\t\t" << std::setw(logIndentation) << "Calculate remainingPercent" << std::endl;
	oss << "\t\t\t" << std::setw(logIndentation) << "remainingPercent from previous run: " << toPercent(remainingPercent) << std::endl;

	float temperature = 0.0f;
	unsigned yesterdayUsage = getRequiredPercentForPreviousDay(dateTime, &temperature);

	oss << "\t\t\t" << std::setw(logIndentation) << "yesterday usage: " << toPercent(yesterdayUsage) << " (" << toCelsius(temperature) << ")" << std::endl;

	int result = remainingPercent;
	result -= static_cast<int>(yesterdayUsage);
	result *= remainingCorrection;

	oss << "\t\t\t" << std::setw(logIndentation) << "yesterday usage: " << toPercent(yesterdayUsage) << std::endl;
	oss << "\t\t\t" << std::setw(logIndentation) << "remaining correction: " << remainingCorrection << std::endl;
	oss << "\t\t\t" << std::setw(logIndentation) << "result: " << toPercent(result) << std::endl;
	return result;
}

///////////////////////////////////////////////////////////////////////////////

unsigned TemperatureDependentSchedulerImpl::calculateRequiredPercentForToday(const int remainingPercent, const DateTime& dateTime, std::ostringstream& oss) const {
	oss << "\t\t" << std::setw(logIndentation) << "Calculate requiredPercentForToday" << std::endl;
	oss << "\t\t\t" << std::setw(logIndentation) << "remainingPercent: " << toPercent(remainingPercent) << std::endl;

	float temperature = 0.0f;
	unsigned forecastedUsage = getRequiredPercentForNextDay(dateTime, &temperature);

	oss << "\t\t\t" << std::setw(logIndentation) << "forecasted usage: " << toPercent(forecastedUsage) << " (" << toCelsius(temperature) << ")" << std::endl;

	int result = static_cast<int>(forecastedUsage);
	result -= remainingPercent;

	if (result < 0) {
		result = 0;
	}

	oss << "\t\t\t" << std::setw(logIndentation) << "forecasted usage: " << toPercent(forecastedUsage) << std::endl;
	oss << "\t\t\t" << std::setw(logIndentation) << "result: " << toPercent(result) << std::endl;
	return static_cast<unsigned>(result);
}

unsigned TemperatureDependentSchedulerImpl::calculateAdjustment(const unsigned requiredPercentForToday, std::ostringstream& oss) const {
	oss << "\t\t" << std::setw(logIndentation) << "Calculate adjustment" << std::endl;
	oss << "\t\t\t" << std::setw(logIndentation) << "requiredPercentForToday: " << toPercent(requiredPercentForToday) << std::endl;

	unsigned result = requiredPercentForToday;

	if (0 != result && nullptr != minAdjustment) {
		result = std::max(result, *minAdjustment);
	}
	oss << "\t\t\t" << std::setw(logIndentation) << "requiredPercent (min): " << toPercent(result) << " (min: " << optionalToString(minAdjustment) << ")" << std::endl;

	if (nullptr != maxAdjustment) {
		result = std::min(result, *maxAdjustment);
	}

	oss << "\t\t\t" << std::setw(logIndentation) << "requiredPercent (max): " << toPercent(result) << " (max: " << optionalToString(maxAdjustment) << ")" << std::endl;
	oss << "\t\t\t" << std::setw(logIndentation) << "result: " << toPercent(result) << std::endl;
	return result;
}

std::unique_ptr<Scheduler::Result> TemperatureDependentSchedulerImpl::process(const LocalDateTime& localDateTime) {

	if (nullptr == temperatureForecast) {
		throw std::logic_error("TemperatureDependentSchedulerImpl::process()  nullptr == temperatureForecast");
	}

	if (nullptr == temperatureHistory) {
		throw std::logic_error("TemperatureDependentSchedulerImpl::process()  nullptr == temperatureHistory");
	}

	const enum Day day = getLastRunDay(localDateTime);

	std::ostringstream oss;
	oss.setf(std::ios::left, std::ios::adjustfield);

	oss << "TemperatureDependentSchedulerImpl::process()" << std::endl;
	oss << "\t" << std::setw(logIndentation) << "last run: " << dayToString(day) << " (" << lastRun.toString("%F %T") << ")" << std::endl;
	oss << "\t" << std::setw(logIndentation) << "remainingPercent from previous run: " << toPercent(remainingPercent) << std::endl;
	oss << "\t" << std::setw(logIndentation) << "requiredPercentForToday from previous run: " << toPercent(requiredPercentForToday) << std::endl;

	lastRun = localDateTime;

	switch (day) {
	case Day::TODAY:
		remainingPercent = calculateRemainingPercentToday(remainingPercent, oss);
		break;
	case Day::YESTERDAY:
		remainingPercent = calculateRemainingPercentYesterday(remainingPercent, localDateTime, oss);
		break;
	case Day::OTHER:
		remainingPercent = calculateRemainingPercentOther(remainingPercent, oss);
		break;
	default:
		throw std::logic_error("Invalid Day");
	}
	oss << "\t" << std::setw(logIndentation) << "remainingPercent: " << toPercent(remainingPercent) << std::endl;


	if (Day::YESTERDAY == day || Day::OTHER == day) {
		requiredPercentForToday = calculateRequiredPercentForToday(remainingPercent, localDateTime, oss);
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

	return std::unique_ptr<Scheduler::Result>(new Scheduler::Result(adjustment));
}

nlohmann::json TemperatureDependentSchedulerImpl::saveTo() const {
	nlohmann::json j;
	j["remainingPercent"] = remainingPercent;
	j["lastRun"] = lastRun.toRawTime();
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
			lastRun = DateTime::create(it.value());
		}
	}
}

void TemperatureDependentSchedulerImpl::setRemainingCorrection(float a) {
	remainingCorrection = a;
}

float TemperatureDependentSchedulerImpl::getRemainingCorrection() const {
	return remainingCorrection;
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

unsigned TemperatureDependentSchedulerImpl::getMinAdjustment() const {
	return (minAdjustment ? *minAdjustment : 0);
}

unsigned TemperatureDependentSchedulerImpl::getMaxAdjustment() const {
	return (maxAdjustment ? *maxAdjustment : 0);
}

TemperatureDependentSchedulerDTO TemperatureDependentSchedulerImpl::toTemperatureDependentSchedulerDto() const {
	return TemperatureDependentSchedulerDTO(
			remainingCorrection,
			minAdjustment ? *minAdjustment : 0, maxAdjustment ? *maxAdjustment : 0
		);
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
	oss << "maxAdjustment=" << optionalToString(maxAdjustment);
	oss << "}";
	return oss.str();
}
