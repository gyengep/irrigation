#pragma once
#include <stdexcept>
#include "KeyValue.h"


class WebServerException : public std::runtime_error {
	unsigned statusCode;
	KeyValue headers;

public:
	WebServerException(unsigned statusCode, const std::string& errorMessage, const KeyValue& headers) :
		std::runtime_error(errorMessage),
		statusCode(statusCode),
		headers(headers)
	{}

	unsigned getStatusCode() const { return statusCode; }
	const char* getErrorMessage() const { return what(); }
	const KeyValue getHeaders() const { return headers; }
};


class HttpInternalServerError : public WebServerException {
public:
	HttpInternalServerError(const std::string& message, const KeyValue& headers = KeyValue()) : WebServerException(500, "Internal Server Error: " + message, headers) {}
};

class HttpNotFount : public WebServerException {
public:
	HttpNotFount(const std::string& message, const KeyValue& headers = KeyValue()) : WebServerException(404, "Not found: " + message, headers) {}
};

class HttpNotImplemented : public WebServerException {
public:
	HttpNotImplemented(const std::string& message, const KeyValue& headers = KeyValue()) : WebServerException(501, "Not implemented: " + message, headers) {}
};

class HttpNotSupported : public WebServerException {
public:
	HttpNotSupported(const std::string& message, const KeyValue& headers = KeyValue()) : WebServerException(505, "HTTP version is not supported: " + message, headers) {}
};

class HttpMethodNotAllowed : public WebServerException {
public:
	HttpMethodNotAllowed(const std::string& message, const KeyValue& headers = KeyValue()) : WebServerException(405, "Method not allowed: " + message, headers) {}
};

