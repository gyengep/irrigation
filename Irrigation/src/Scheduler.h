/*
 * Watering.h
 *
 *  Created on: 2016. júl. 3.
 *      Author: Rendszergazda
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <array>
#include <ctime>

#include "common.h"


class SpecifiedScheduler_enableDay_Test;

class Scheduler {
public:
	virtual ~Scheduler() {}
	virtual bool isScheduled(const std::tm& timeinfo) const = 0;
};

enum Days {
	SUNDAY,
	MONDAY,
	TUESDAY,
	WEDNESDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY
};

class SpecifiedScheduler : public Scheduler {

	friend SpecifiedScheduler_enableDay_Test;

	static const unsigned DAY_COUNT = 7;

	typedef std::array<bool, DAY_COUNT> Days;

	Days days;

public:
	SpecifiedScheduler();
	virtual ~SpecifiedScheduler();

	void enableDay(IdType id, bool enable);
	bool isDayEnabled(IdType id) const;

	virtual bool isScheduled(const std::tm& timeinfo) const;
};


#endif /* SCHEDULER_H_ */
