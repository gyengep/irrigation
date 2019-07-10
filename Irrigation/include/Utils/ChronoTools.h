#pragma once
#include <chrono>


class TimeConverter {
	typedef std::chrono::duration<int64_t, std::ratio<24 * 3600>> Days;

	Days days;
	std::chrono::hours hours;
	std::chrono::minutes minutes;
	std::chrono::seconds seconds;
	std::chrono::milliseconds millis;

public:
	TimeConverter(std::chrono::milliseconds ms);

	size_t getDays() const { return days.count(); }
	size_t getHours() const { return hours.count(); }
	size_t getMinutes() const { return minutes.count(); }
	size_t getSeconds() const { return seconds.count(); }
	size_t getMillis() const { return millis.count(); }
};

std::chrono::seconds abs(const std::chrono::seconds& t);
std::chrono::milliseconds abs(const std::chrono::milliseconds& t);
