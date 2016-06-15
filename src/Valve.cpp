/*
 * Valve.cpp
 *
 *  Created on: 2016. máj. 14.
 *      Author: Rendszergazda
 */

#include "common.h"
#include <stdexcept>
//#include <wiringPi.h>
#include "Valve.h"

Valve::Valve(int pin) :
		pin(pin) {
//	pinMode (pin, OUTPUT);
}

Valve::~Valve() {
	open(false);
}

void Valve::init() {
/*
	if (wiringPiSetup() == -1) {
		throw std::runtime_error("GPIO initialization FAILED");
	}
	*/
}

void Valve::open(bool open) {
//	digitalWrite(pin, open ? 1 : 0);
}
