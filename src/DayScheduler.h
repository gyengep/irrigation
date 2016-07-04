/*
 * Watering.h
 *
 *  Created on: 2016. júl. 3.
 *      Author: Rendszergazda
 */

#ifndef DAYSCHEDULER_H_
#define DAYSCHEDULER_H_

class DayScheduler {
public:
	virtual ~DayScheduler() {}
	virtual bool isDayScheduled(std::tm* timeinfo) const = 0;
};

#endif /* DAYSCHEDULER_H_ */
