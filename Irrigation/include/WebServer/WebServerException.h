#pragma once
#include <stdexcept>
#include <string>
#include "KeyValue.h"



class WebServerException : public std::runtime_error {
	const unsigned statusCode;
	const std::string statusMessage;
	const KeyValue headers;

public:
	WebServerException(
		unsigned statusCode,
		const std::string& statusMessage,
		const std::string& errorMessage,
		const KeyValue& headers
	) :
		std::runtime_error(errorMessage),
		statusCode(statusCode),
		statusMessage(statusMessage),
		headers(headers)
	{}

	unsigned getStatusCode() const { return statusCode; }
	const char* getStatusMessage() const { return statusMessage.c_str(); }
	const char* getErrorMessage() const { return what(); }
	const KeyValue& getHeaders() const { return headers; }
};

