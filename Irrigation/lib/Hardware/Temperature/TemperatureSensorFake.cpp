#include "TemperatureSensorFake.h"
#include "TemperatureException.h"


TemperatureSensorFake::TemperatureSensorFake() {
}

TemperatureSensorFake::~TemperatureSensorFake() {
}

float TemperatureSensorFake::readValueFromSensor() {
	throw TemperatureException("TemperatureSensorFake::readValueFromSensor()");
}
