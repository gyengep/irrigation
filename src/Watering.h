/*
 * Watering.h
 *
 *  Created on: 2016. j�l. 3.
 *      Author: Rendszergazda
 */

#ifndef WATERING_H_
#define WATERING_H_

class Watering {
public:
	virtual ~Watering() {}
	virtual bool isDayScheduled(std::tm* timeinfo) const = 0;
};

#endif /* WATERING_H_ */
