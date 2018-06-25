#include "Valves.h"
#include <sstream>
#include <vector>
#include "Exceptions/Exceptions.h"

using namespace std;


mutex Valves::createMutex;
unique_ptr<Valves> Valves::instance;

Valves& Valves::getInstance() {
	if (nullptr == instance) {
		lock_guard<std::mutex> lock(createMutex);

		if (nullptr == instance) {
			instance.reset(new Valves());
		}
	}

	return *instance;
}

void Valves::setNewInstance(Valves* newInstance) {
	lock_guard<std::mutex> lock(createMutex);
	instance.reset(newInstance);
}

///////////////////////////////////////////////////////////////////////////////

Valves::Valves() {
}

Valves::~Valves() {
}

void Valves::resetAll() {
	const size_t size = 7;
	size_t allValves[size] { 0, 1, 2, 3, 4, 5, 6 };
	activate(allValves, size, false);
}

void Valves::activate(size_t valveID, bool active) {
	lock_guard<std::mutex> lock(mutex);

	activatePin(valveID, active);
}

void Valves::activate(size_t* valveIDs, size_t size, bool active) {
	lock_guard<std::mutex> lock(mutex);

	for (size_t i = 0; i < size; ++i) {
		size_t valveID = valveIDs[i];
		if (pins.size() <= valveID) {
			throw IndexOutOfBoundsException(
					"Valve index shall be less than " + to_string(pins.size()) +
					", while actual value is " + to_string(valveID));
		}
	}

	for (size_t i = 0; i < size; ++i) {
		activatePin(valveIDs[i], active);
	}
}

void Valves::activatePin(size_t valveID, bool active) {
	setPin(pins[valveID], active ? 1 : 0);
}

#ifdef __arm__

#include <wiringPi.h>

void Valves::init() {
	if (wiringPiSetup() == -1) {
		throw runtime_error("GPIO initialization FAILED");
	}

	for (size_t i = 0; i < PIN_COUNT; ++i) {
		pinMode(pins[i], OUTPUT);
	}
}

void Valves::setPin(int pin, int mode) {
	digitalWrite(pin, mode);
}

#else

void Valves::init() {
}

void Valves::setPin(int pin, int mode) {
}


#endif // __arm__
