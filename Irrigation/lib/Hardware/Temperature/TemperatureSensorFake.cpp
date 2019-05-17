#include "TemperatureSensorFake.h"
#include "TemperatureException.h"


TemperatureSensor_Fake::TemperatureSensor_Fake() {
}

TemperatureSensor_Fake::~TemperatureSensor_Fake() {
}

float TemperatureSensor_Fake::readValueFromSensor() {
	throw TemperatureException("TemperatureSensor_Fake::readValueFromSensor()");
}
