#include "TemperatureSensorReaderFake.h"
#include "TemperatureException.h"


TemperatureSensorReaderFake::TemperatureSensorReaderFake() {
}

TemperatureSensorReaderFake::~TemperatureSensorReaderFake() {
}

float TemperatureSensorReaderFake::read() {
	throw TemperatureException("TemperatureSensorReaderFake::read()");
}
