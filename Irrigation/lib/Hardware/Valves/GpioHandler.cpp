#include "GpioHandler.h"
#include "ValveConfig.h"

#if VALVE_COUNT != 7
#error Invalid valve count
#endif

using namespace std;


GpioHandler::GpioHandler() {
}

GpioHandler::~GpioHandler() {
}

GpioHandler& GpioHandler::getInstance() {
	static GpioHandler instance;
	return instance;
}

#ifdef __arm__
#include <wiringPi.h>

void GpioHandler::init() {
	if (wiringPiSetup() == -1) {
		throw runtime_error("GPIO initialization FAILED");
	}

	pinMode(VALVE0_PIN, OUTPUT);
	pinMode(VALVE1_PIN, OUTPUT);
	pinMode(VALVE2_PIN, OUTPUT);
	pinMode(VALVE3_PIN, OUTPUT);
	pinMode(VALVE4_PIN, OUTPUT);
	pinMode(VALVE5_PIN, OUTPUT);
	pinMode(VALVE6_PIN, OUTPUT);
}

void GpioHandler::setPin(int pin, int mode) {
	digitalWrite(pin, mode);
}

#else

void GpioHandler::init() {
}

void GpioHandler::setPin(int pin, int mode) {
}

#endif


