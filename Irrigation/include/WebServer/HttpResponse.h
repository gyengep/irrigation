#pragma once
#include <memory>
#include <string>
#include <microhttpd.h>
#include "KeyValue.h"



class HttpResponse {
public:
	class Builder;

private:
	const unsigned statusCode;
	const std::string message;
	KeyValue headers;

	HttpResponse(unsigned statusCode, const std::string& message, const KeyValue& headers) :
		statusCode(statusCode),
		message(message),
		headers(headers)
	{
	}

public:

	const std::string& getMessage() const { return message; }
	const KeyValue& gerHeaders() const { return headers; }
	unsigned getStatusCode() const { return statusCode; }
};


class HttpResponse::Builder {
	unsigned statusCode;
	std::string body;
	KeyValue headers;

public:
	Builder();
	~Builder();

	Builder& setStatusCode(unsigned statusCode);
	Builder& setBody(const std::string& body);
	Builder& addHeaders(const KeyValue& headers);
	Builder& addHeader(const std::string& key, const std::string& value);

	std::unique_ptr<HttpResponse> build();
};
