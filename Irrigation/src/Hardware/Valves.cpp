#include "common.h"
#include "Valves.h"


std::mutex Valves::createMutex;
std::unique_ptr<Factory<Valves>> Valves::factory(new ValvesFactory());
std::unique_ptr<Valves> Valves::instance;

const std::array<int, Valves::PIN_COUNT> Valves::pins {
	VALVE0_PIN,
	VALVE1_PIN,
	VALVE2_PIN,
	VALVE3_PIN,
	VALVE4_PIN,
	VALVE5_PIN,
	VALVE6_PIN
};

Valves& Valves::getInstance() {
	if (nullptr == instance) {
		std::lock_guard<std::mutex> lock(createMutex);

		if (nullptr == instance) {
			instance.reset(factory->create());
		}
	}

	return *instance;
}

void Valves::setFactory(Factory<Valves>* valvesFactory) {
	std::lock_guard<std::mutex> lock(createMutex);

	instance.reset(nullptr);
	factory.reset(valvesFactory);
}

Valves::Valves() {
}

Valves::~Valves() {
}

void Valves::activate_notSafe(size_t valveID, bool active) {
	if (PIN_COUNT <= valveID) {
		throw std::out_of_range("Invalid valveID: " + std::to_string(valveID));
	}

#if not defined(IRRIGATION_TEST)
#ifdef __arm__
	digitalWrite(pins[idx], active ? 1 : 0);
#endif // __arm__
#endif
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
