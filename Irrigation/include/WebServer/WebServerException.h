#pragma once
#include <stdexcept>
#include "KeyValue.h"


class WebServerException : public std::runtime_error {
	const unsigned statusCode;
	const KeyValue headers;

public:
	WebServerException(unsigned statusCode, const std::string& message = std::string(), const KeyValue& headers = KeyValue()) :
		std::runtime_error(message),
		statusCode(statusCode),
		headers(headers)
	{}

	unsigned getStatusCode() const { return statusCode; }
	const char* getErrorMessage() const { return what(); }
	const KeyValue getHeaders() const { return headers; }
};


class HttpNotAcceptable : public WebServerException {
public:
	HttpNotAcceptable() : WebServerException(406) {}
};


class HttpInternalServerError : public WebServerException {
public:
	HttpInternalServerError() : WebServerException(500, "Internal Server Error", KeyValue({{"Content-Type", "text/plain"}})) {}
};

