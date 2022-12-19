#pragma once
#include <list>
#include <string>
#include "Utils/OstreamInsert.h"


namespace AAA {

	class Response {
		const long responseCode;

	    std::list<std::string> headers;
	    std::string body;

	public:
		Response(long responseCode);

		Response& setBody(const std::string& body);
		Response& addHeaders(const std::list<std::string>& headers);
		Response& addHeader(const std::string& header);
		Response& setContentType(const std::string& contentType);

		const std::string toString() const;

		long getResponseCode() const 						{ return responseCode; };
	    const std::list<std::string>& getHeaders() const 	{ return headers; }
		const std::string& getBody() const 					{ return body; }
	};

};

OSTREAM_INSERT(AAA::Response);
