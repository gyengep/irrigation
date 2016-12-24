/*
 * Valve.cpp
 *
 *  Created on: 2016. máj. 14.
 *      Author: Rendszergazda
 */

#include "Common.h"
#include "Valve.h"


#ifdef __arm__

#include <wiringPi.h>

Valve::Valve(int pin) : pin(pin) {
	pinMode (pin, OUTPUT);
}

Valve::~Valve() {
	open(false);
}

void Valve::init() {
	if (wiringPiSetup() == -1) {
		throw std::runtime_error("GPIO initialization FAILED");
	}
}

void Valve::open(bool open) {
	digitalWrite(pin, open ? 1 : 0);
}

#else

Valve::Valve(int pin) : pin(pin) {
}

Valve::~Valve() {
}

void Valve::init() {
}

void Valve::open(bool open) {
}

#endif // __arm__
