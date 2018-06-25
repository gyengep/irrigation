#pragma once
#include <stdexcept>
#include <string>


class NoSuchElementException : public std::runtime_error {
public:
	NoSuchElementException(const std::string& what) :
		std::runtime_error(what) {}
};
