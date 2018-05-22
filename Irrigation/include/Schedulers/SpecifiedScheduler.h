#pragma once
#include <array>
#include "Schedulers/Scheduler.h"



class SpecifiedScheduler : public Scheduler {
public:

	enum Days {
		SUNDAY,
		MONDAY,
		TUESDAY,
		WEDNESDAY,
		THURSDAY,
		FRIDAY,
		SATURDAY
	};

private:
	static const unsigned DAY_COUNT = 7;
	typedef std::array<bool, DAY_COUNT> DayArray;

	DayArray days;

public:
	SpecifiedScheduler();
	virtual ~SpecifiedScheduler();

	void enableDay(Days day, bool enable);
	bool isDayEnabled(Days day) const;

	virtual bool isDayScheduled(const std::tm& timeinfo) const;
};
