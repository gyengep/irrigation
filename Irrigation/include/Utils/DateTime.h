#pragma once
#include <chrono>
#include <ctime>
#include <memory>
#include <stdexcept>
#include <string>


class Timefunc {
public:
	virtual ~Timefunc() = default;
	virtual std::time_t getTime() const = 0;
};


class DefaultTimefunc : public Timefunc {
public:
	virtual ~DefaultTimefunc() = default;
	virtual std::time_t getTime() const override;
};

///////////////////////////////////////////////////////////////////////////////

class DateTime {
	static std::shared_ptr<Timefunc> timefunc;

protected:
	std::time_t rawtime;

public:
	DateTime(const DateTime& other);
	explicit DateTime(const std::time_t& rawtime);
	virtual ~DateTime() = default;

	bool operator==(const DateTime& other) const;
	bool operator!=(const DateTime& other) const;
	bool operator<=(const DateTime& other) const;
	bool operator>=(const DateTime& other) const;
	bool operator<(const DateTime& other) const;
	bool operator>(const DateTime& other) const;

	DateTime add(const std::chrono::seconds& second) const;
	DateTime addHours(int hour) const;
	DateTime addMinutes(int minute) const;
	DateTime addSeconds(int second) const;

	const std::time_t& toRawtime() const;

	static DateTime now();
	static void setTimefunc(const std::shared_ptr<Timefunc>& timefunc);
	static void resetTimefunc();
};

///////////////////////////////////////////////////////////////////////////////

template <class T>
class ZonedDateTime : public DateTime {
	mutable std::unique_ptr<std::tm> timeinfo;

	std::tm* getTimeinfo() const;

public:
	ZonedDateTime(const DateTime& other);
	ZonedDateTime(const ZonedDateTime<T>& other);
	explicit ZonedDateTime(const std::time_t& rawtime);

	ZonedDateTime& operator=(const ZonedDateTime<T>& other);

	DateTime addDays(int day) const;

	int getYears() const;
	int getMonths() const;
	int getDays() const;
	int getHours() const;
	int getMinutes() const;
	int getSeconds() const;

	std::string toString() const;
	std::string toString(const char* format) const;
	std::string toString(const std::string& format) const;

	static ZonedDateTime now();
	static ZonedDateTime create(int year, int month, int day, int hour, int minute, int second);
	static void checkDate(int year, int month, int day);
	static void checkTime(int hour, int minute, int second);
};

///////////////////////////////////////////////////////////////////////////////

class LocalDateTimeConversion {
public:
	static void toTimeinfo(std::tm* timeinfo, const std::time_t& rawtime);
	static std::time_t fromTimeinfo(std::tm* timeinfo);
};

class UtcDateTimeConversion {
public:
	static void toTimeinfo(std::tm* timeinfo, const std::time_t& rawtime);
	static std::time_t fromTimeinfo(std::tm* timeinfo);
};

typedef ZonedDateTime<LocalDateTimeConversion> LocalDateTime;
typedef ZonedDateTime<UtcDateTimeConversion> UtcDateTime;

///////////////////////////////////////////////////////////////////////////////

template <class T>
ZonedDateTime<T>::ZonedDateTime(const std::time_t& rawtime) :
	DateTime(rawtime)
{
}

template <class T>
ZonedDateTime<T>::ZonedDateTime(const DateTime& other) :
	DateTime(other)
{
}

template <class T>
ZonedDateTime<T>::ZonedDateTime(const ZonedDateTime<T>& other) :
	DateTime(other)
{
}

template <class T>
ZonedDateTime<T> ZonedDateTime<T>::now() {
	return ZonedDateTime(DateTime::now());
}

template <class T>
ZonedDateTime<T> ZonedDateTime<T>::create(int year, int month, int day, int hour, int minute, int second) {
	checkDate(year, month, day);
	checkTime(hour, minute, second);

	std::tm timeinfo {0};

	timeinfo.tm_year = year - 1900;
	timeinfo.tm_mon = month - 1;
	timeinfo.tm_mday = day;
	timeinfo.tm_hour = hour;
	timeinfo.tm_min = minute;
	timeinfo.tm_sec = second;
	timeinfo.tm_isdst = -1;

	return ZonedDateTime(T::fromTimeinfo(&timeinfo));
}

template <class T>
std::tm* ZonedDateTime<T>::getTimeinfo() const {
	if (!timeinfo) {
		timeinfo.reset(new std::tm{0});
		T::toTimeinfo(timeinfo.get(), rawtime);
	}

	return timeinfo.get();
}

template <class T>
std::string ZonedDateTime<T>::toString() const {
	return toString("%F %T");
}

template <class T>
std::string ZonedDateTime<T>::toString(const std::string& format) const {
	return toString(format.c_str());
}

template <class T>
std::string ZonedDateTime<T>::toString(const char* format) const {
	static const int bufferSize = 100;
	char buffer[bufferSize];

	std::strftime(buffer, bufferSize, format, getTimeinfo());

	return std::string(buffer);
}

template <class T>
int ZonedDateTime<T>::getYears() const {
	return getTimeinfo()->tm_year + 1900;
}

template <class T>
int ZonedDateTime<T>::getMonths() const {
	return getTimeinfo()->tm_mon + 1;
}

template <class T>
int ZonedDateTime<T>::getDays() const {
	return getTimeinfo()->tm_mday;
}

template <class T>
int ZonedDateTime<T>::getHours() const {
	return getTimeinfo()->tm_hour;
}

template <class T>
int ZonedDateTime<T>::getMinutes() const {
	return getTimeinfo()->tm_min;
}

template <class T>
int ZonedDateTime<T>::getSeconds() const {
	return getTimeinfo()->tm_sec;
}

template <class T>
ZonedDateTime<T>& ZonedDateTime<T>::operator=(const ZonedDateTime<T>& other) {
	if (this != &other) {
		timeinfo.reset();
		DateTime::operator=(other);
	}

	return *this;
}

template <class T>
DateTime ZonedDateTime<T>::addDays(int day) const {
	std::tm timeinfo {0};
	T::toTimeinfo(&timeinfo, rawtime);
	timeinfo.tm_mday += day;
	timeinfo.tm_isdst = -1;

	return DateTime(T::fromTimeinfo(&timeinfo));
}

template <class T>
void ZonedDateTime<T>::checkDate(int year, int month, int day) {
	const int yearMin = 1970;
	const int yearMax = 2038;

	if (year < yearMin || yearMax < year) {
		throw std::runtime_error("Year number must be between " + std::to_string(yearMin) + " and " + std::to_string(yearMax) + " (current value is " + std::to_string(year) + ")");
	}

	if (month < 1 || 12 < month) {
		throw std::runtime_error("Month number must be between 1 and 12 (current value is " + std::to_string(month) + ")");
	}

	if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
		if (day < 1 || 31 < day) {
			throw std::runtime_error("Day must be between 1 and 31 for this month (current value is " + std::to_string(day) + ")");
		}
	}
	else if (month == 4 || month == 6 || month == 9 || month == 11) {
		if (day < 1 || 30 < day) {
			throw std::runtime_error("Day must be between 1 and 30 for this month (current value is " + std::to_string(day) + ")");
		}
	}
	else { /*month == 2*/
		if ((year % 4 == 0) && (day < 1 || 29 < day)) {
			throw std::runtime_error("Day must be between 1 and 29 for this month (current value is " + std::to_string(day) + ")");
		}

		if ((year % 4 != 0) && (day < 1 || 28 < day)) {
			throw std::runtime_error("Day must be between 1 and 28 for this month (current value is " + std::to_string(day) + ")");
		}
	}
}

template <class T>
void ZonedDateTime<T>::checkTime(int hour, int minute, int second) {
	if (hour < 0 || 23 < hour) {
		throw std::runtime_error("Hour value must be between 0 and 23 (current value is " + std::to_string(hour) + ")");
	}

	if (minute < 0 || 59 < minute) {
		throw std::runtime_error("Minute value must be between 0 and 59 (current value is " + std::to_string(minute) + ")");
	}

	if (second < 0 || 59 < second) {
		throw std::runtime_error("Second value must be between 0 and 59 (current value is " + std::to_string(second) + ")");
	}
}
