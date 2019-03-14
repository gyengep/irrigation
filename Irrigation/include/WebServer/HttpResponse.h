#pragma once
#include <microhttpd.h>
#include <string>
#include "KeyValue.h"



class HttpResponse {
	const unsigned statusCode;
	const std::string message;
	KeyValue headers;

public:
	HttpResponse(unsigned statusCode = MHD_HTTP_OK) :
		statusCode(statusCode),
		message(),
		headers()
	{
	}

	HttpResponse(unsigned statusCode, const std::string& message, const std::string& contentType) :
		statusCode(statusCode),
		message(message),
		headers()
	{
		addHeader("Content-Type", contentType);
	}

	HttpResponse(unsigned statusCode, const std::string& message, const KeyValue& headers) :
		statusCode(statusCode),
		message(message),
		headers(headers)
	{
	}

	void addHeader(const std::string& key, const std::string& value) {
		headers.insert(std::make_pair(key, value));
	}

	const std::string& getMessage() const { return message; }
	const KeyValue& gerHeaders() const { return headers; }
	unsigned getStatusCode() const { return statusCode; }
};
