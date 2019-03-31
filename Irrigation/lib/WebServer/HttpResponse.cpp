#include "HttpResponse.h"

using namespace std;



HttpResponse::Builder::Builder() {
}

HttpResponse::Builder::~Builder() {
}

HttpResponse::Builder& HttpResponse::Builder::setStatusCode(unsigned statusCode) {
	this->statusCode = statusCode;
	return *this;
}

HttpResponse::Builder& HttpResponse::Builder::setMessage(const std::string& message) {
	this->message = message;
	addHeader("Content-Length", to_string(message.length()));
	return *this;
}

HttpResponse::Builder& HttpResponse::Builder::setHeaders(const KeyValue& headers) {
	this->headers = headers;
	return *this;
}

HttpResponse::Builder& HttpResponse::Builder::addHeader(const std::string& key, const std::string& value) {
	this->headers.insert(make_pair(key, value));
	return *this;
}


unique_ptr<HttpResponse> HttpResponse::Builder::build() {
	return unique_ptr<HttpResponse>(new HttpResponse(statusCode, message, headers));
}
