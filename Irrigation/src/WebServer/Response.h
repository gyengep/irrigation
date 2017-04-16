#pragma once
#include <microhttpd.h>

class Request;
class WebServer;


class Response {
	friend WebServer;

	MHD_Response* response;
	MHD_Connection* connection;
	unsigned statusCode;

	void send();

public:
	Response(const Request& request, const std::string& message, int statusCode = MHD_HTTP_OK);
	~Response();
};
