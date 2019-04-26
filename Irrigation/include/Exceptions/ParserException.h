#pragma once
#include <stdexcept>
#include <string>


class ParserException : public std::runtime_error {
public:
	ParserException(const std::string& what) :
		std::runtime_error(what) {}
};
