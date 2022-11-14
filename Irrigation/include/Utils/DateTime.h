#pragma once
#include <chrono>
#include <ctime>
#include <memory>
#include <string>
#include "OstreamInsert.h"


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

	DateTime(const std::time_t& rawtime);

public:
	virtual ~DateTime() = default;

	DateTime& operator=(const DateTime& other);

	bool operator==(const DateTime& other) const;
	bool operator!=(const DateTime& other) const;
	bool operator<=(const DateTime& other) const;
	bool operator>=(const DateTime& other) const;
	bool operator<(const DateTime& other) const;
	bool operator>(const DateTime& other) const;

	DateTime operator+(const std::chrono::seconds& seconds) const;
	DateTime operator-(const std::chrono::seconds& seconds) const;
	std::chrono::seconds operator-(const DateTime& other) const;

	DateTime addHours(int hours) const;
	DateTime addMinutes(int minutes) const;
	DateTime addSeconds(int seconds) const;

	const std::time_t& toRawTime() const { return rawtime; }

	static DateTime now();
	static DateTime epoch();
	static DateTime create(const std::time_t& rawtime);

	static void setTimefunc(const std::shared_ptr<Timefunc>& timefunc);
	static void resetTimefunc();
};

///////////////////////////////////////////////////////////////////////////////

class ZonedDateTime : public DateTime {
protected:
	class Converter;

private:
	const std::shared_ptr<Converter> converter;
	mutable std::unique_ptr<std::tm> timeinfo;

	std::tm* getTimeinfo() const;

	static std::time_t createRawTime(const std::shared_ptr<Converter>& converter, int years, int months, int days, int hours, int minutes, int seconds);

protected:
	ZonedDateTime(const std::shared_ptr<Converter>& converter, const DateTime& other);
	ZonedDateTime(const std::shared_ptr<Converter>& converter, int years, int months, int days, int hours, int minutes, int seconds);

public:
	virtual ~ZonedDateTime() = default;

	ZonedDateTime& operator=(const ZonedDateTime& other);

	DateTime addDays(int days) const;

	int getYears() const;
	int getMonths() const;
	int getDays() const;
	int getHours() const;
	int getMinutes() const;
	int getSeconds() const;
	int getDayOfWeek() const;

	std::string toString() const;
	std::string toString(const char* format) const;
	std::string toString(const std::string& format) const;

	static void checkDate(int years, int months, int days);
	static void checkTime(int hours, int minutes, int seconds);
};

class ZonedDateTime::Converter {
public:
	virtual ~Converter() = default;
	virtual void toTimeinfo(std::tm* timeinfo, const std::time_t& rawtime) const = 0;
	virtual std::time_t fromTimeinfo(std::tm* timeinfo) const = 0;
};

///////////////////////////////////////////////////////////////////////////////

class LocalDateTime : public ZonedDateTime {
	class Converter;

public:
	LocalDateTime(const DateTime& other);
	LocalDateTime(const LocalDateTime& other);
	LocalDateTime(int years, int months, int days);
	LocalDateTime(int years, int months, int days, int hours, int minutes, int seconds);
	virtual ~LocalDateTime() = default;

	LocalDateTime& operator=(const LocalDateTime& other);

	static LocalDateTime now();
};

class LocalDateTime::Converter : public ZonedDateTime::Converter {
public:
	virtual void toTimeinfo(std::tm* timeinfo, const std::time_t& rawtime) const override;
	virtual std::time_t fromTimeinfo(std::tm* timeinfo) const override;
};

///////////////////////////////////////////////////////////////////////////////

class UtcDateTime : public ZonedDateTime {
	class Converter;

public:
	UtcDateTime(const DateTime& other);
	UtcDateTime(const UtcDateTime& other);
	UtcDateTime(int years, int months, int days, int hours, int minutes, int seconds);
	virtual ~UtcDateTime() = default;

	UtcDateTime& operator=(const UtcDateTime& other);

	static UtcDateTime now();
};

class UtcDateTime::Converter : public ZonedDateTime::Converter {
public:
	virtual void toTimeinfo(std::tm* timeinfo, const std::time_t& rawtime) const override;
	virtual std::time_t fromTimeinfo(std::tm* timeinfo) const override;
};

///////////////////////////////////////////////////////////////////////////////

OSTREAM_INSERT(LocalDateTime);
OSTREAM_INSERT(UtcDateTime);
