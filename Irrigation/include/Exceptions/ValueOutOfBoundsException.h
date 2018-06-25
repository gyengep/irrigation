#pragma once
#include <stdexcept>
#include <string>


class ValueOutOfBoundsException : public std::runtime_error {
public:
	ValueOutOfBoundsException(const std::string& what) :
		std::runtime_error(what) {}
};
