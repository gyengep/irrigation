#pragma once

#include <stdexcept>


class WebServerException : public std::runtime_error {
	int statusCode;

public:
	WebServerException(int statusCode) :
		std::runtime_error(""),
		statusCode(statusCode)
	{}

	int getStatusCode() const { return statusCode; }
};
