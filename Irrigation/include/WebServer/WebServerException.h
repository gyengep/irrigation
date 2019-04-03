#pragma once
#include <stdexcept>
#include "KeyValue.h"



class WebServerException : public std::runtime_error {
	const unsigned statusCode;
	const KeyValue headers;

public:
	WebServerException(unsigned statusCode, const std::string& message, const KeyValue& headers) :
		std::runtime_error(message),
		statusCode(statusCode),
		headers(headers)
	{}

	unsigned getStatusCode() const { return statusCode; }
	const char* getErrorMessage() const { return what(); }
	const KeyValue& getHeaders() const { return headers; }
};

