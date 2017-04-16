#pragma once

#include <microhttpd.h>
#include <map>
#include <memory>
#include <set>

class Response;
class Request;


class WebServer {
	static int accessHandlerCallback(void *cls, struct MHD_Connection *connection,
		const char *url, const char *method, const char *version,
		const char *upload_data, size_t *upload_data_size, void **con_cls);

	static std::set<WebServer*> instances;
	static std::map<int, std::string> errorMessages;

	struct MHD_Daemon *daemon;

	static std::string getErrorMessage(int errorCode);
	static std::string getErrorPage(int errorCode);

public:
	WebServer(uint16_t port);
	virtual ~WebServer();

	virtual Response* onRequest(const Request& request) = 0;
};
