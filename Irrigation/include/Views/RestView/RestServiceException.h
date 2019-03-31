#pragma once
#include <memory>
#include <string>
#include "WebServer/KeyValue.h"
#include "WebServer/WebServerException.h"
#include "ErrorWriter.h"


class RestServiceException : public WebServerException {
public:
	RestServiceException(const std::shared_ptr<ErrorWriter>& errorWriter, unsigned statusCode, const std::string& message) :
		WebServerException(statusCode, errorWriter->toString(statusCode, message), KeyValue({{"Content-Type", errorWriter->contentType()}}))
	{
	}
};


class RestBadRequest : public RestServiceException {
public:
	RestBadRequest(const std::shared_ptr<ErrorWriter>& errorWriter, const std::string& message) :
		RestServiceException(errorWriter, 400, message)
	{
	}
};

class RestNotFound : public RestServiceException {
public:
	RestNotFound(const std::shared_ptr<ErrorWriter>& errorWriter, const std::string& message) :
		RestServiceException(errorWriter, 404, message)
	{
	}
};

class RestMethodNotAllowed : public RestServiceException {
public:
	RestMethodNotAllowed(const std::shared_ptr<ErrorWriter>& errorWriter, const std::string& message) :
		RestServiceException(errorWriter, 405, message)
	{
	}
};

class RestNotAcceptable : public RestServiceException {
public:
	RestNotAcceptable(const std::shared_ptr<ErrorWriter>& errorWriter, const std::string& message) :
		RestServiceException(errorWriter, 406, message)
	{
	}
};

class RestUnsupportedMediaType : public RestServiceException {
public:
	RestUnsupportedMediaType(const std::shared_ptr<ErrorWriter>& errorWriter, const std::string& message) :
		RestServiceException(errorWriter, 415, message)
	{
	}
};

class RestInternalServerError : public RestServiceException {
public:
	RestInternalServerError(const std::shared_ptr<ErrorWriter>& errorWriter, const std::string& message) :
		RestServiceException(errorWriter, 500, message)
	{
	}
};

