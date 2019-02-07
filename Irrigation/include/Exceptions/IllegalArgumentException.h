#pragma once
#include <stdexcept>
#include <string>


class IllegalArgumentException : public std::runtime_error {
public:
	IllegalArgumentException(const std::string& what) :
		std::runtime_error(what) {}
};
