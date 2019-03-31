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

HttpResponse::Builder& HttpResponse::Builder::setBody(const std::string& body, const std::string& contentType) {
	this->body = body;
	addHeader("Content-Type", contentType);
	return *this;
}

HttpResponse::Builder& HttpResponse::Builder::addHeader(const std::string& key, const std::string& value) {
	this->headers.insert(make_pair(key, value));
	return *this;
}


unique_ptr<HttpResponse> HttpResponse::Builder::build() {
	return unique_ptr<HttpResponse>(new HttpResponse(statusCode, body, headers));
}
