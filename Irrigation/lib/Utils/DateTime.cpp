#include "DateTime.h"
#include <stdexcept>


std::time_t DefaultTimefunc::getTime() const {
	return std::time(nullptr);
}

///////////////////////////////////////////////////////////////////////////////

std::shared_ptr<Timefunc> DateTime::timefunc = std::make_shared<DefaultTimefunc>();

DateTime::DateTime(const DateTime& other) :
	rawtime(other.rawtime)
{
}

DateTime::DateTime(const std::time_t& rawtime) :
	rawtime(rawtime)
{
}

DateTime DateTime::addHours(int hour) const {
	return DateTime(rawtime + (hour * 60 * 60));
}

DateTime DateTime::addMinutes(int minute) const {
	return DateTime(rawtime + (minute * 60));
}

DateTime DateTime::addSeconds(int second) const {
	return DateTime(rawtime + second);
}

bool DateTime::operator==(const DateTime& other) const {
	return (rawtime == other.rawtime);
}

const std::time_t& DateTime::toRawtime() const {
	return rawtime;
}

DateTime DateTime::now() {
	return DateTime(timefunc->getTime());
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



void LocalDateTimeConversion::toTimeinfo(std::tm* timeinfo, const std::time_t& rawtime) {
	localtime_r(&rawtime, timeinfo);
}

std::time_t LocalDateTimeConversion::fromTimeinfo(std::tm* timeinfo) {
	return mktime(timeinfo);
}

void UtcDateTimeConversion::toTimeinfo(std::tm* timeinfo, const std::time_t& rawtime) {
	gmtime_r(&rawtime, timeinfo);
}

std::time_t UtcDateTimeConversion::fromTimeinfo(std::tm* timeinfo) {
	return timegm(timeinfo);
}
