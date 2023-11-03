#pragma once
#include <memory>
#include <string>
#include "WebServer/KeyValue.h"
#include "WebServer/WebServerException.h"
#include "ErrorWriter.h"


class RestServiceException : public WebServerException {
public:
	RestServiceException(
			const std::shared_ptr<ErrorWriter>& errorWriter, unsigned statusCode, const std::string& genericMessage, const std::string& detailedMessage) :
		WebServerException(
				statusCode,
				genericMessage,
				errorWriter->toString(statusCode, genericMessage, detailedMessage),
				KeyValue({{"Content-Type", errorWriter->contentType()}})
			)
	{
	}
};


class RestBadRequest : public RestServiceException {
public:
	RestBadRequest(const std::shared_ptr<ErrorWriter>& errorWriter, const std::string& message) :
		RestServiceException(errorWriter, 400, "Bad Request", message)
	{
	}
};

class RestNotFound : public RestServiceException {
public:
	RestNotFound(const std::shared_ptr<ErrorWriter>& errorWriter, const std::string& message) :
		RestServiceException(errorWriter, 404, "Not Found", message)
	{
	}
};

class RestMethodNotAllowed : public RestServiceException {
public:
	RestMethodNotAllowed(const std::shared_ptr<ErrorWriter>& errorWriter, const std::string& message) :
		RestServiceException(errorWriter, 405, "Method Not Allowed", message)
	{
	}
};

class RestNotAcceptable : public RestServiceException {
public:
	RestNotAcceptable(const std::shared_ptr<ErrorWriter>& errorWriter, const std::string& message) :
		RestServiceException(errorWriter, 406, "Not Acceptable", message)
	{
	}
};

class RestUnsupportedMediaType : public RestServiceException {
public:
	RestUnsupportedMediaType(const std::shared_ptr<ErrorWriter>& errorWriter, const std::string& message) :
		RestServiceException(errorWriter, 415, "Unsupported Media Type", message)
	{
	}
};

class RestUnprocessableContent : public RestServiceException {
public:
	RestUnprocessableContent(const std::shared_ptr<ErrorWriter>& errorWriter, const std::string& message) :
		RestServiceException(errorWriter, 422, "Unprocessable Content", message)
	{
	}
};

class RestInternalServerError : public RestServiceException {
public:
	RestInternalServerError(const std::shared_ptr<ErrorWriter>& errorWriter, const std::string& message) :
		RestServiceException(errorWriter, 500, "Internal Server Error", message)
	{
	}
};

