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
	const std::string statusMessage;
	const std::string body;
	KeyValue headers;

	HttpResponse(unsigned statusCode, const std::string& statusMessage, const std::string& body, const KeyValue& headers) :
		statusCode(statusCode),
		statusMessage(statusMessage),
		body(body),
		headers(headers)
	{
	}

public:

	const std::string& getBody() const { return body; }
	const KeyValue& gerHeaders() const { return headers; }
	unsigned getStatusCode() const { return statusCode; }
	const std::string& getStatusMessage() const { return statusMessage; }
};


class HttpResponse::Builder {
	unsigned statusCode;
	std::string statusMessage;
	std::string body;
	KeyValue headers;

public:
	Builder();
	~Builder();

	Builder& setStatus(unsigned statusCode, const std::string& statusMessage);
	Builder& setBody(const std::string& body);
	Builder& addHeaders(const KeyValue& headers);
	Builder& addHeader(const std::string& key, const std::string& value);

	std::unique_ptr<HttpResponse> build();
};
