#include "Valves.h"


std::mutex Valves::createMutex;
std::unique_ptr<Valves> Valves::instance;


Valves& Valves::getInstance() {
	if (nullptr == instance) {
		std::lock_guard<std::mutex> lock(createMutex);

		if (nullptr == instance) {
			instance.reset(new Valves());
		}
	}

	return *instance;
}

Valves::Valves() {
}

Valves::~Valves() {
}

void Valves::activate(size_t valveID, bool active) {
	activate(&valveID, 1, active);
}

void Valves::activate(const size_t* valveIDs, size_t size, bool active) {
	std::lock_guard<std::mutex> lock(mutex);

	for (size_t i = 0; i < size; ++i) {
		size_t valveID = valveIDs[i];
		if (pins.size() <= valveID) {
			throw std::out_of_range("Invalid valveID: " + std::to_string(valveID));
		}
	}

	for (size_t i = 0; i < size; ++i) {
		size_t valveID = valveIDs[i];
		setPin(pins[valveID], active ? 1 : 0);
	}
}

#ifdef __arm__

#include <wiringPi.h>

void Valves::init() {
	if (wiringPiSetup() == -1) {
		throw std::runtime_error("GPIO initialization FAILED");
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
