#include "Response.h"
#include <sstream>

using namespace AAA;


Response::Response(long responseCode):
	responseCode(responseCode)
{
}

Response& Response::setBody(const std::string& body) {
	this->body = body;
	return *this;
}

Response& Response::addHeaders(const std::list<std::string>& headers) {
	this->headers.insert(this->headers.end(), headers.begin(), headers.end());
	return *this;
}

Response& Response::addHeader(const std::string& header) {
	this->headers.push_back(header);
	return *this;
}

Response& Response::setContentType(const std::string& contentType) {
	addHeader("Content-Type: " + contentType + "\r\n");
	return *this;
}

const std::string Response::toString() const {
	std::ostringstream oss;

	oss  << "code: " << responseCode << std::endl;

	for (const auto& header : headers) {
		oss  << "header: " << header;
	}

	oss  << "body: " << body << std::endl;
	return oss.str();
}
