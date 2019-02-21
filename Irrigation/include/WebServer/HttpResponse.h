#pragma once
#include <map>
#include <microhttpd.h>
#include <string>

typedef std::map<std::string, std::string> KeyValue;


class HttpResponse {
	const std::string message;
	const KeyValue responseHeaders;
	const unsigned statusCode;

public:
	HttpResponse(const std::string& message, const KeyValue& responseHeaders = KeyValue(), int statusCode = MHD_HTTP_OK) :
		message(message),
		responseHeaders(responseHeaders),
		statusCode(statusCode)
	{
	}

	const std::string& getMessage() const { return message; }
	const KeyValue& gerHeaders() const { return responseHeaders; }
	unsigned getStatusCode() const { return statusCode; }
};
