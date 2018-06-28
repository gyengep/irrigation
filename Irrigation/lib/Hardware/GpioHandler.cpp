#include "GpioHandler.h"

using namespace std;


mutex GpioHandler::createMutex;
shared_ptr<GpioHandler> GpioHandler::instance;

const shared_ptr<GpioHandler> GpioHandler::getInstancePtr() {
	if (nullptr == instance) {
		lock_guard<std::mutex> lock(createMutex);

		if (nullptr == instance) {
			instance.reset(new GpioHandler());
		}
	}

	return instance;
}



#ifdef __arm__

#include <wiringPi.h>

void GpioHandler::init() {
	if (wiringPiSetup() == -1) {
		throw runtime_error("GPIO initialization FAILED");
	}

#if VALVE_COUNT != 7
#error Invalid valve count
#endif

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


#endif // __arm__
