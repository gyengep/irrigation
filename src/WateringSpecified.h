/*
 * WateringSpecified.h
 *
 *  Created on: 2016. júl. 3.
 *      Author: Rendszergazda
 */

#ifndef WATERINGSPECIFIED_H_
#define WATERINGSPECIFIED_H_

#include <array>
#include <atomic>
#include <ctime>

#include "Watering.h"


class Watering_Specified : public Watering {

	static const unsigned DAY_COUNT = 7;

	typedef std::array<std::pair<IdType, std::atomic<bool>>, DAY_COUNT> Days;


	Days days;

public:
	Watering_Specified();
	virtual ~Watering_Specified();

	void enableDay(IdType id, bool enable);
	bool isDayEnabled(IdType id) const;

	virtual bool isDayScheduled(std::tm* timeinfo) const;
};

#endif /* WATERINGSPECIFIED_H_ */
