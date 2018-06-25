#pragma once
#include <cstring>
#include <stdexcept>


class IOException : public std::runtime_error {
public:
	IOException(int errorCode) :
		std::runtime_error(strerror(errorCode)) {}
};
