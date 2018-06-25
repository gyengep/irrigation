#pragma once
#include <stdexcept>
#include <string>


class IndexOutOfBoundsException : public std::runtime_error {
public:
	IndexOutOfBoundsException(const std::string& what) :
		std::runtime_error(what) {}
};
