#include "Common.h"
#include "Valves.h"


std::unique_ptr<Valves> Valves::instance;
const std::array<int, Valves::PIN_COUNT> Valves::pins {
	VALVE0_PIN,
	VALVE1_PIN,
	VALVE2_PIN,
	VALVE3_PIN,
	VALVE4_PIN,
	VALVE5_PIN,
	VALVE6_PIN};

Valves& Valves::getInstance() {
	if (nullptr == instance) {
		static std::mutex mutex;
		std::lock_guard<std::mutex> lock(mutex);

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

void Valves::activate_notSafe(size_t valveID, bool active) {
	if (PIN_COUNT <= valveID) {
		throw std::out_of_range("Invalid valveID: " + std::to_string(valveID));
	}

#ifdef __arm__
	digitalWrite(pins[idx], active ? 1 : 0);
#endif // __arm__
}

void Valves::activate(size_t valveID, bool active) {
	std::lock_guard<std::mutex> lock(mutex);
	activate_notSafe(valveID, active);
}

void Valves::activate(size_t valveID1, size_t valveID2, bool active) {
	std::lock_guard<std::mutex> lock(mutex);
	activate_notSafe(valveID1, active);
	activate_notSafe(valveID2, active);
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


#else

void Valves::init() {
}



#endif // __arm__
