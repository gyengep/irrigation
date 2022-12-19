#pragma once
#include <string>
#include "Response.h"


namespace AAA {

	class Request {
		const std::string method;
		const std::string url;

		std::string header;
		std::string body;

	public:
		Request(const std::string& method, const std::string&  url);

		Request& setBody(const std::string& body);
		Request& setContentType(const std::string& contentType);
		Request& addHeader(const std::string& header);

		Response execute();
	};

}
