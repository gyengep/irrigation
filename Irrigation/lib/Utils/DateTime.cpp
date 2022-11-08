#include "DateTime.h"
#include <stdexcept>
#include <iostream>


std::time_t DefaultTimefunc::getTime() const {
	return std::time(nullptr);
}

///////////////////////////////////////////////////////////////////////////////

std::shared_ptr<Timefunc> DateTime::timefunc = std::make_shared<DefaultTimefunc>();


DateTime::DateTime(const std::time_t& rawtime) :
	rawtime(rawtime)
{
}

DateTime& DateTime::operator=(const DateTime& other) {
	if (this !=  &other) {
		this->rawtime = other.rawtime;
	}

	return *this;
}

DateTime DateTime::operator+(const std::chrono::seconds& seconds) const {
	return DateTime(rawtime + seconds.count());
}

DateTime DateTime::operator-(const std::chrono::seconds& seconds) const {
	return DateTime(rawtime - seconds.count());
}

std::chrono::seconds DateTime::operator-(const DateTime& other) const {
	return std::chrono::seconds(rawtime - other.rawtime);
}

DateTime DateTime::addHours(int hours) const {
	return this->operator+(std::chrono::hours(hours));
}

DateTime DateTime::addMinutes(int minutes) const {
	return this->operator+(std::chrono::minutes(minutes));
}

DateTime DateTime::addSeconds(int seconds) const {
	return this->operator+(std::chrono::seconds(seconds));
}

bool DateTime::operator==(const DateTime& other) const {
	return (rawtime == other.rawtime);
}

bool DateTime::operator!=(const DateTime& other) const {
	return (rawtime != other.rawtime);
}

bool DateTime::operator<=(const DateTime& other) const {
	return (rawtime <= other.rawtime);
}

bool DateTime::operator>=(const DateTime& other) const {
	return (rawtime >= other.rawtime);
}

bool DateTime::operator<(const DateTime& other) const {
	return (rawtime < other.rawtime);
}

bool DateTime::operator>(const DateTime& other) const {
	return (rawtime > other.rawtime);
}

DateTime DateTime::now() {
	return DateTime(timefunc->getTime());
}

DateTime DateTime::epoch() {
	return DateTime(0);
}

DateTime DateTime::create(const std::time_t& rawtime) {
	return DateTime(rawtime);
}

void DateTime::setTimefunc(const std::shared_ptr<Timefunc>& timefunc) {
	if (nullptr == timefunc) {
		throw std::invalid_argument("DateTime::setTimefunc() nullptr == timefunc");
	}

	DateTime::timefunc = timefunc;
}

void DateTime::resetTimefunc() {
	timefunc = std::make_shared<DefaultTimefunc>();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

ZonedDateTime::ZonedDateTime(const std::shared_ptr<Converter>& converter, const DateTime& other) :
	DateTime(other),
	converter(converter),
	timeinfo()
{
}

ZonedDateTime::ZonedDateTime(const std::shared_ptr<Converter>& converter, int years, int months, int days, int hours, int minutes, int seconds) :
	DateTime(createRawTime(converter, years, months, days, hours, minutes, seconds)),
	converter(converter),
	timeinfo()
{
}

ZonedDateTime& ZonedDateTime::operator=(const ZonedDateTime& other) {
	if (this != &other) {
		timeinfo.reset();
		DateTime::operator=(other);
	}

	return *this;
}

std::time_t ZonedDateTime::createRawTime(const std::shared_ptr<Converter>& converter, int years, int months, int days, int hours, int minutes, int seconds) {
	checkDate(years, months, days);
	checkTime(hours, minutes, seconds);

	std::tm timeinfo {0};

	timeinfo.tm_year = years - 1900;
	timeinfo.tm_mon = months - 1;
	timeinfo.tm_mday = days;
	timeinfo.tm_hour = hours;
	timeinfo.tm_min = minutes;
	timeinfo.tm_sec = seconds;
	timeinfo.tm_isdst = -1;

	return converter->fromTimeinfo(&timeinfo);
}

std::tm* ZonedDateTime::getTimeinfo() const {
	if (nullptr == timeinfo) {
		timeinfo.reset(new std::tm{0});
		converter->toTimeinfo(timeinfo.get(), rawtime);
	}

	return timeinfo.get();
}

std::string ZonedDateTime::toString() const {
	return toString("%F %T");
}

std::string ZonedDateTime::toString(const std::string& format) const {
	return toString(format.c_str());
}

std::string ZonedDateTime::toString(const char* format) const {
	static const int bufferSize = 100;
	char buffer[bufferSize];

	std::strftime(buffer, bufferSize, format, getTimeinfo());

	return std::string(buffer);
}

int ZonedDateTime::getYears() const {
	return getTimeinfo()->tm_year + 1900;
}

int ZonedDateTime::getMonths() const {
	return getTimeinfo()->tm_mon + 1;
}

int ZonedDateTime::getDays() const {
	return getTimeinfo()->tm_mday;
}

int ZonedDateTime::getDayOfWeek() const {
	return (getTimeinfo()->tm_wday + 6) % 7;
}

int ZonedDateTime::getHours() const {
	return getTimeinfo()->tm_hour;
}

int ZonedDateTime::getMinutes() const {
	return getTimeinfo()->tm_min;
}

int ZonedDateTime::getSeconds() const {
	return getTimeinfo()->tm_sec;
}

DateTime ZonedDateTime::addDays(int days) const {
	std::tm timeinfo {0};

	converter->toTimeinfo(&timeinfo, rawtime);

	timeinfo.tm_mday += days;
	timeinfo.tm_isdst = -1;

	return DateTime::create(converter->fromTimeinfo(&timeinfo));
}

void ZonedDateTime::checkDate(int years, int months, int days) {
	const int yearMin = 1970;
	const int yearMax = 2038;

	if (years < yearMin || yearMax < years) {
		throw std::runtime_error("Year number must be between " + std::to_string(yearMin) + " and " + std::to_string(yearMax) + " (current value is " + std::to_string(years) + ")");
	}

	if (months < 1 || 12 < months) {
		throw std::runtime_error("Month number must be between 1 and 12 (current value is " + std::to_string(months) + ")");
	}

	if (months == 1 || months == 3 || months == 5 || months == 7 || months == 8 || months == 10 || months == 12) {
		if (days < 1 || 31 < days) {
			throw std::runtime_error("Day must be between 1 and 31 for this months (current value is " + std::to_string(days) + ")");
		}
	}
	else if (months == 4 || months == 6 || months == 9 || months == 11) {
		if (days < 1 || 30 < days) {
			throw std::runtime_error("Day must be between 1 and 30 for this month (current value is " + std::to_string(days) + ")");
		}
	}
	else { /*months == 2*/
		if ((years % 4 == 0) && (days < 1 || 29 < days)) {
			throw std::runtime_error("Day must be between 1 and 29 for this month (current value is " + std::to_string(days) + ")");
		}

		if ((years % 4 != 0) && (days < 1 || 28 < days)) {
			throw std::runtime_error("Day must be between 1 and 28 for this month (current value is " + std::to_string(days) + ")");
		}
	}
}

void ZonedDateTime::checkTime(int hours, int minutes, int seconds) {
	if (hours < 0 || 23 < hours) {
		throw std::runtime_error("Hour value must be between 0 and 23 (current value is " + std::to_string(hours) + ")");
	}

	if (minutes < 0 || 59 < minutes) {
		throw std::runtime_error("Minute value must be between 0 and 59 (current value is " + std::to_string(minutes) + ")");
	}

	if (seconds < 0 || 59 < seconds) {
		throw std::runtime_error("Second value must be between 0 and 59 (current value is " + std::to_string(seconds) + ")");
	}
}

///////////////////////////////////////////////////////////////////////////////

LocalDateTime::LocalDateTime(const DateTime& other) :
	ZonedDateTime(std::make_shared<LocalDateTime::Converter>(), other)
{
}

 LocalDateTime::LocalDateTime(const LocalDateTime& other) :
	ZonedDateTime(std::make_shared<LocalDateTime::Converter>(), other)
{
}

 LocalDateTime::LocalDateTime(int years, int months, int days) :
 	ZonedDateTime(std::make_shared<LocalDateTime::Converter>(), years, months, days, 0, 0, 0)
 {
 }

 LocalDateTime::LocalDateTime(int years, int months, int days, int hours, int minutes, int seconds) :
 	ZonedDateTime(std::make_shared<LocalDateTime::Converter>(), years, months, days, hours, minutes, seconds)
 {
 }

LocalDateTime& LocalDateTime::operator=(const LocalDateTime& other) {
	if (this != &other) {
		ZonedDateTime::operator=(other);
	}

	return *this;
}

LocalDateTime LocalDateTime::now() {
	return LocalDateTime(DateTime::now());
}

///////////////////////////////////////////////////////////////////////////////

void LocalDateTime::Converter::toTimeinfo(std::tm* timeinfo, const std::time_t& rawtime) const {
	localtime_r(&rawtime, timeinfo);
}

std::time_t LocalDateTime::Converter::fromTimeinfo(std::tm* timeinfo) const {
	return mktime(timeinfo);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

UtcDateTime::UtcDateTime(const DateTime& other) :
	ZonedDateTime(std::make_shared<UtcDateTime::Converter>(), other)
{
}

UtcDateTime::UtcDateTime(const UtcDateTime& other) :
	ZonedDateTime(std::make_shared<UtcDateTime::Converter>(), other)
{
}

UtcDateTime::UtcDateTime(int years, int months, int days, int hours, int minutes, int seconds) :
	ZonedDateTime(std::make_shared<UtcDateTime::Converter>(), years, months, days, hours, minutes, seconds)
{
}

UtcDateTime& UtcDateTime::operator=(const UtcDateTime& other) {
	if (this != &other) {
		ZonedDateTime::operator=(other);
	}

	return *this;
}

UtcDateTime UtcDateTime::now() {
	return UtcDateTime(DateTime::now());
}

///////////////////////////////////////////////////////////////////////////////

void UtcDateTime::Converter::toTimeinfo(std::tm* timeinfo, const std::time_t& rawtime) const {
	gmtime_r(&rawtime, timeinfo);
}

std::time_t UtcDateTime::Converter::fromTimeinfo(std::tm* timeinfo) const {
	return timegm(timeinfo);
}
