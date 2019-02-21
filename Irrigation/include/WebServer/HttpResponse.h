#pragma once
#include <map>
#include <microhttpd.h>
#include <string>

typedef std::map<std::string, std::string> Parameters;


class HttpResponse {
	const std::string message;
	const Parameters responseHeaders;
	const unsigned statusCode;

public:
	HttpResponse(const std::string& message, const Parameters& responseHeaders = Parameters(), int statusCode = MHD_HTTP_OK) :
		message(message),
		responseHeaders(responseHeaders),
		statusCode(statusCode)
	{
	}

	const std::string& getMessage() const { return message; }
	const Parameters& gerHeaders() const { return responseHeaders; }
	unsigned getStatusCode() const { return statusCode; }
};
