/*
 * Valve.h
 *
 *  Created on: 2016. máj. 14.
 *      Author: Rendszergazda
 */

#ifndef VALVE_H_
#define VALVE_H_


class Valve {
	const int pin;

public:
	Valve(int pin);
	virtual ~Valve();

	void open(bool open);
	
	static void init();
};

#endif /* VALVE_H_ */
