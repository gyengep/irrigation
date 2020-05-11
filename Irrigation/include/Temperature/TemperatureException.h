#pragma once
#include <stdexcept>


class TemperatureException : public std::runtime_error {
public:
	TemperatureException(const std::string& what) :
		std::runtime_error(what) {}
};

