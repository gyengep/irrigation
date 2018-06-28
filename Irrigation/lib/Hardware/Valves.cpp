#include "Valves.h"
#include "GpioHandler.h"
#include <sstream>
#include <vector>
#include "Exceptions/Exceptions.h"

using namespace std;


mutex Valves::createMutex;
shared_ptr<Valves> Valves::instance;

const shared_ptr<Valves> Valves::getInstancePtr() {
	if (nullptr == instance) {
		lock_guard<std::mutex> lock(createMutex);

		if (nullptr == instance) {
			instance.reset(new Valves(GpioHandler::getInstancePtr()));
		}
	}

	return instance;
}

///////////////////////////////////////////////////////////////////////////////

Valves::Valves(shared_ptr<GpioHandler> gpioHandler) :
	gpioHandler(gpioHandler)
{
}

Valves::~Valves() {
}

void Valves::checkId(size_t valveID) {
	if (pins.size() <= valveID) {
		throw IndexOutOfBoundsException(
				"Valve index shall be less than " + to_string(pins.size()) +
				", while actual value is " + to_string(valveID));
	}
}

void Valves::activateWithoutLock(size_t valveID, bool active) {
	if (gpioHandler != nullptr) {
		gpioHandler->setPin(pins[valveID], active ? 1 : 0);
	}
}

void Valves::activate(size_t valveID, bool active) {
	lock_guard<std::mutex> lock(mutex);

	checkId(valveID);
	activateWithoutLock(valveID, active);
}

void Valves::activate(const size_t* valveIDs, size_t size, bool active) {
	lock_guard<std::mutex> lock(mutex);

	for (size_t i = 0; i < size; ++i) {
		checkId(valveIDs[i]);
	}

	for (size_t i = 0; i < size; ++i) {
		activateWithoutLock(valveIDs[i], active);
	}
}


