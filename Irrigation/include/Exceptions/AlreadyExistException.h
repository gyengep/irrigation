#pragma once
#include <stdexcept>
#include <string>


class AlreadyExistException : public std::runtime_error {
public:
	AlreadyExistException(const std::string& what) :
		std::runtime_error(what) {}
};
