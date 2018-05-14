#pragma once
#include <stdexcept>



class InvalidDayException : public std::runtime_error {
public:
	explicit InvalidDayException() : std::runtime_error("Invalid day") {}
};

