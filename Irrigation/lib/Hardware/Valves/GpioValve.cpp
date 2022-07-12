#include "GpioValve.h"
#include "ValveConfig.h"
#include "Exceptions/Exceptions.h"

using namespace std;

const std::vector<std::shared_ptr<GpioValve>> GpioValve::valves({
	std::make_shared<GpioValve>(VALVE0_PIN),
	std::make_shared<GpioValve>(VALVE1_PIN),
	std::make_shared<GpioValve>(VALVE2_PIN),
	std::make_shared<GpioValve>(VALVE3_PIN),
	std::make_shared<GpioValve>(VALVE4_PIN),
	std::make_shared<GpioValve>(VALVE5_PIN),
	std::make_shared<GpioValve>(VALVE6_PIN)
});


#ifdef __arm__
#include <wiringPi.h>

void GpioValve::init() {
	if (wiringPiSetup() == -1) {
		throw runtime_error("GPIO initialization FAILED");
	}

	for (const auto& valve : valves) {
		pinMode(valve->pin, OUTPUT);
	}
}

#else

void GpioValve::init() {
}

#endif


std::vector<std::shared_ptr<Valve>> GpioValve::getValves() {
	return std::vector<std::shared_ptr<Valve>>(valves.begin(), valves.end());
}


GpioValve::GpioValve(int pin) :
	pin(pin)
{
}

GpioValve::~GpioValve() {
}

void GpioValve::setPin(int mode) {
#ifdef __arm__
	digitalWrite(pin, mode);
#endif
}

void GpioValve::activate() {
	setPin(1);
}

void GpioValve::deactivate() {
	setPin(0);
}
