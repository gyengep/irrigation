#pragma once
#include <stdexcept>
#include <string>


class InterruptedException : public std::runtime_error {
public:
	InterruptedException(const std::string& what) :
		std::runtime_error(what) {}
};
