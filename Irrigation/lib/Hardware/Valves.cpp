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
	vector<size_t> allValves { 0, 1, 2, 3, 4, 5, 6 };
	activate(allValves, false);
}

void Valves::activate(size_t valveID, bool active) {
	vector<size_t> valveIDs { valveID };
	activate(valveIDs, active);
}

void Valves::activate(const vector<size_t>& valveIDs, bool active) {
	lock_guard<std::mutex> lock(mutex);

	for (size_t i = 0; i < valveIDs.size(); ++i) {
		size_t valveID = valveIDs[i];
		if (pins.size() <= valveID) {
			throw IndexOutOfBoundsException(
					"Valve index shall be less than " + to_string(pins.size()) +
					", while actual value is " + to_string(valveID));
		}
	}

	for (size_t i = 0; i < valveIDs.size(); ++i) {
		size_t valveID = valveIDs[i];
		setPin(pins[valveID], active ? 1 : 0);
	}
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
