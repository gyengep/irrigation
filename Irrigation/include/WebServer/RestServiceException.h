#pragma once
#include <memory>
#include <string>
#include "WebServerException.h"
#include "KeyValue.h"


class RestServiceException : public WebServerException {
public:
	RestServiceException(const std::unique_ptr<ErrorWriter>& errorWriter, unsigned statusCode, const std::string& message, const KeyValue& headers) :
		WebServerException(statusCode, errorWriter->to(message, statusCode), headers)
	{
	}
};


class RestHttpNotFount : public RestServiceException {
public:
	RestHttpNotFount(const std::unique_ptr<ErrorWriter>& errorWriter, const std::string& message, const KeyValue& headers) :
		RestServiceException(errorWriter, 404, message, headers)
	{
	}
};

