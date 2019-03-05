#pragma once
#include <microhttpd.h>
#include <string>
#include "KeyValue.h"



class HttpResponse {
	const std::string message;
	const KeyValue responseHeaders;
	const unsigned statusCode;

public:
	HttpResponse(const std::string& message, const KeyValue& responseHeaders = KeyValue(), unsigned statusCode = MHD_HTTP_OK) :
		message(message),
		responseHeaders(responseHeaders),
		statusCode(statusCode)
	{
	}

	const std::string& getMessage() const { return message; }
	const KeyValue& gerHeaders() const { return responseHeaders; }
	unsigned getStatusCode() const { return statusCode; }
};
