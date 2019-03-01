#pragma once
#include <stdexcept>


class WebServerException : public std::runtime_error {
	unsigned statusCode;

public:
	WebServerException(int statusCode, const std::string& errorMessage) :
		std::runtime_error(errorMessage),
		statusCode(statusCode)
	{}

	unsigned getStatusCode() const { return statusCode; }
	const char* getErrorMessage() const { return what(); }
};


class HttpInternalServerError : public WebServerException {
public:
	HttpInternalServerError(const std::string& message) : WebServerException(500, "Internal Server Error: " + message) {}
};

class HttpNotFount : public WebServerException {
public:
	HttpNotFount(const std::string& message) : WebServerException(404, "Not found: " + message) {}
};

class HttpNotImplemented : public WebServerException {
public:
	HttpNotImplemented(const std::string& message) : WebServerException(501, "Not implemented: " + message) {}
};

class HttpNotSupported : public WebServerException {
public:
	HttpNotSupported(const std::string& message) : WebServerException(505, "HTTP version is not supported: " + message) {}
};

class HttpMethodNotAllowed : public WebServerException {
public:
	HttpMethodNotAllowed(const std::string& message) : WebServerException(405, "Method not allowed: " + message) {}
};

