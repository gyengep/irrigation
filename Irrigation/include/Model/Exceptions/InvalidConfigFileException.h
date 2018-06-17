#pragma once
#include <stdexcept>
#include <string>



class InvalidConfigFileException : public std::runtime_error {
public:
	InvalidConfigFileException(const std::string what) : std::runtime_error(what) {}
};
