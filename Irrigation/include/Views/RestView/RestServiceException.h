#pragma once
#include <memory>
#include <string>
#include "WebServer/KeyValue.h"
#include "WebServer/WebServerException.h"
#include "ErrorWriter.h"


class RestServiceException : public WebServerException {
public:
	RestServiceException(const std::unique_ptr<ErrorWriter>& errorWriter, unsigned statusCode, const std::string& message, const std::string& contentType) :
		WebServerException(statusCode, errorWriter->to(message, statusCode), KeyValue({{"Content-Type", contentType}}))
	{
	}
};


class RestNotAcceptable : public RestServiceException {
public:
	RestNotAcceptable(const std::unique_ptr<ErrorWriter>& errorWriter, const std::string& message, const std::string& contentType) :
		RestServiceException(errorWriter, 404, message, contentType)
	{
	}
};

class RestUnsupportedMediaType : public RestServiceException {
public:
	RestUnsupportedMediaType(const std::unique_ptr<ErrorWriter>& errorWriter, const std::string& message, const std::string& contentType) :
		RestServiceException(errorWriter, 415, message, contentType)
	{
	}
};

class RestInternalServerError : public RestServiceException {
public:
	RestInternalServerError(const std::unique_ptr<ErrorWriter>& errorWriter, const std::string& message, const std::string& contentType) :
		RestServiceException(errorWriter, 500, message, contentType)
	{
	}
};

class RestNotFound : public RestServiceException {
public:
	RestNotFound(const std::unique_ptr<ErrorWriter>& errorWriter, const std::string& message, const std::string& contentType) :
		RestServiceException(errorWriter, 404, message, contentType)
	{
	}
};

class RestBadRequest : public RestServiceException {
public:
	RestBadRequest(const std::unique_ptr<ErrorWriter>& errorWriter, const std::string& message, const std::string& contentType) :
		RestServiceException(errorWriter, 400, message, contentType)
	{
	}
};

class RestMethodNotAllowed : public RestServiceException {
public:
	RestMethodNotAllowed(const std::unique_ptr<ErrorWriter>& errorWriter, const std::string& message, const std::string& contentType) :
		RestServiceException(errorWriter, 405, message, contentType)
	{
	}
};

