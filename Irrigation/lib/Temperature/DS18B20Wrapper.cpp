#include "DS18B20Handler.h"
#include <stdexcept>


DS18B20Handler::DS18B20Handler() {
}

DS18B20Handler::~DS18B20Handler() {
}

float DS18B20Handler::readCurrentTemperature() const {
	throw std::runtime_error("Method not implemented DS18B20Handler::readCurrentTemperature()");
}

std::string DS18B20Handler::getSensorName() const {
	return "DS18B20";
}
