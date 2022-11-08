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
	TimeConverter(const std::chrono::milliseconds& ms);

	int64_t getDays() const { return days.count(); }
	int64_t getHours() const { return hours.count(); }
	int64_t getMinutes() const { return minutes.count(); }
	int64_t getSeconds() const { return seconds.count(); }
	int64_t getMillis() const { return millis.count(); }
};
