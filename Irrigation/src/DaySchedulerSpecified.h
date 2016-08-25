/*
 * WateringSpecified.h
 *
 *  Created on: 2016. júl. 3.
 *      Author: Rendszergazda
 */

#ifndef DAYSCHEDULERSPECIFIED_H_
#define DAYSCHEDULERSPECIFIED_H_

#include <array>
#include <ctime>

#include "DayScheduler.h"


class DayScheduler_Specified : public DayScheduler {

	static const unsigned DAY_COUNT = 7;

	typedef std::array<std::pair<IdType, bool>, DAY_COUNT> Days;

	Days days;

public:
	DayScheduler_Specified();
	virtual ~DayScheduler_Specified();

	void enableDay(IdType id, bool enable);
	bool isDayEnabled(IdType id) const;

	virtual bool isDayScheduled(std::tm* timeinfo) const;
};

#endif /* DAYSCHEDULERSPECIFIED_H_ */
