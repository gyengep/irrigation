#pragma once
#include <stdexcept>
#include <string>



class ConfigFileOpenException : public std::runtime_error {
public:
	ConfigFileOpenException(const std::string what) : std::runtime_error(what) {}
};


