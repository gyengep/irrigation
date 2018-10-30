#include "GpioValve.h"
#include "GpioHandler.h"
#include "ValveConfig.h"
#include "Exceptions/IndexOutOfBoundsException.h"

using namespace std;


GpioValve::GpioValve(int pin) : pin(pin) {
}

GpioValve::~GpioValve() {
}

void GpioValve::activate() {
	GpioHandler::getInstance().setPin(pin, 1);
}

void GpioValve::deactivate() {
	GpioHandler::getInstance().setPin(pin, 0);
}

///////////////////////////////////////////////////////////////////////////////

unique_ptr<Valve> GpioValveFactory::createValve(size_t id) {
	static const array<int, VALVE_COUNT> pins {
		VALVE0_PIN,
		VALVE1_PIN,
		VALVE2_PIN,
		VALVE3_PIN,
		VALVE4_PIN,
		VALVE5_PIN,
		VALVE6_PIN
	};

	if (pins.size() <= id) {
		throw IndexOutOfBoundsException(
				"Valve index shall be less than " + to_string(pins.size()) +
				", while actual value is " + to_string(id));
	}

	return unique_ptr<Valve>(new GpioValve(pins[id]));
}
