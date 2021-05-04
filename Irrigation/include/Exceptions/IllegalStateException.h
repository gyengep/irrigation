#pragma once
#include <stdexcept>
#include <string>


class IllegalStateException : public std::logic_error {
public:
	IllegalStateException(const std::string& what) :
		std::logic_error(what) {}
};
