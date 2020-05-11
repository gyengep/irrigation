#include "DS18B20Wrapper.h"
#include <stdexcept>


DS18B20Wrapper::DS18B20Wrapper() {
}

DS18B20Wrapper::~DS18B20Wrapper() {
}

float DS18B20Wrapper::readCurrentTemperature() const {
	return ds18B20TemperatureSensor.read();
}

std::string DS18B20Wrapper::getSensorName() const {
	return "DS18B20";
}
