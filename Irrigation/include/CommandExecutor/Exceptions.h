#pragma once
#include <stdexcept>



class UnknownCommandException : public std::runtime_error {
public:
	UnknownCommandException(const std::string& command) :
		std::runtime_error("Unknown command: '" + command + "'") {}
};
