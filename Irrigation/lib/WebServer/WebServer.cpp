#include "Common.h"
#include "Response.h"
#include "Request.h"
#include "WebServer.h"
#include "WebServerException.h"

#include <cstring>
#include <sstream>


std::set<WebServer*> WebServer::instances;
std::map<int, std::string> WebServer::errorMessages {
	{ MHD_HTTP_INTERNAL_SERVER_ERROR, "Internal Server Error" },
	{ MHD_HTTP_NOT_FOUND, "Not Found" },
	{ MHD_HTTP_NOT_IMPLEMENTED, "Not Implemented"},
	{ MHD_HTTP_HTTP_VERSION_NOT_SUPPORTED, "HTTP Version Not Supported" },
};


WebServer::WebServer(uint16_t port)
{
	instances.insert(this);

	struct MHD_OptionItem ops[] = {
//		{ MHD_OPTION_CONNECTION_LIMIT, 100, NULL },
//		{ MHD_OPTION_CONNECTION_TIMEOUT, 10, NULL },
		{ MHD_OPTION_END, 0, NULL }
	};

	daemon = MHD_start_daemon(
			MHD_USE_SELECT_INTERNALLY, port,
			NULL, NULL,
			WebServer::accessHandlerCallback, this,
			MHD_OPTION_ARRAY, ops, MHD_OPTION_END);

	MHD_set_panic_func(onFatalError, NULL);

	if (NULL == daemon) {
		instances.erase(this);
		throw std::runtime_error("Can not create webserver");
	}

	LOGGER.info("WebServer started on port: %hd", port);
}

WebServer::~WebServer() {
	MHD_stop_daemon(daemon);

	auto it = instances.find(this);
	if (instances.end() != it) {
		instances.erase(it);
	} else {
		LOGGER.warning("WebServer already deleted: 0x%08X", this);
	}

	LOGGER.info("WebServer stopped");
}

void WebServer::onFatalError(void *cls, const char *file, unsigned int line, const char *reason) {
	LOGGER.error("WebServer stopped working (%s:%u)! reason: %s", file, line, (reason ? reason : "<null>"));
}

int WebServer::iterateOnGetParameters(void *cls, enum MHD_ValueKind kind, const char *key, const char *value) {
	LOGGER.trace("GET parameter: \"%s\" - \"%s\"", key, value);

	Request::Parameters* requestParameters = static_cast<Request::Parameters*>(cls);
	requestParameters->insert(std::make_pair(std::string(key), std::string(value)));
	return MHD_YES;
}

int WebServer::accessHandlerCallback(void *cls,
		struct MHD_Connection* connection, const char* url, const char* method, const char* version,
		const char* upload_data, size_t* upload_data_size, void** con_cls) {

	LOGGER.debug("HTTP request received");

	LOGGER.trace("connection: %p", connection);
	LOGGER.trace("url: %s", url);
	LOGGER.trace("method: %s", method);
	LOGGER.trace("version: %s", version);
	LOGGER.trace("upload data size: %u", *upload_data_size);

	try {

		Request::Parameters requestParameters;
		MHD_get_connection_values(connection, MHD_GET_ARGUMENT_KIND, iterateOnGetParameters, &requestParameters);

		Request request(connection, url, method, version, upload_data, upload_data_size, requestParameters);
		std::unique_ptr<Response> response;

		try {
			WebServer* webServer = static_cast<WebServer*>(cls);
			if (WebServer::instances.end() == WebServer::instances.find(webServer)) {
				LOGGER.warning("Invalid WebServer callback");
				throw WebServerException(MHD_HTTP_INTERNAL_SERVER_ERROR);
			}


			response.reset(webServer->onRequest(request));
			LOGGER.debug("sending HTTP response");

		} catch (WebServerException& e) {
			LOGGER.debug("HTTP error: %d - %s", e.getStatusCode(), getErrorMessage(e.getStatusCode()).c_str());
			response.reset(new Response(request, getErrorPage(e.getStatusCode()), e.getStatusCode()));
		} catch (std::exception& e) {
			LOGGER.warning(e.what());
			response.reset(new Response(request, getErrorPage(MHD_HTTP_INTERNAL_SERVER_ERROR), MHD_HTTP_INTERNAL_SERVER_ERROR));
		}

		response->send();
		return MHD_YES;

	} catch (std::exception& e) {
		LOGGER.warning(e.what());
		LOGGER.debug("connection closed");
		return MHD_NO;
	}
}

std::string WebServer::getErrorMessage(int errorCode) {
	std::ostringstream o;
	std::string errorMessage;
	auto it = errorMessages.find(errorCode);

	if (errorMessages.end() == it) {
		errorMessage = "HTTP error";
		LOGGER.warning("HTTP error code not found: %d", errorCode);
	} else {
		errorMessage = it->second;
	}

	return errorMessage;
}

std::string WebServer::getErrorPage(int errorCode) {
	std::ostringstream o;
	std::string errorMessage = getErrorMessage(errorCode);

	o << "<html><head><title>" << errorMessage << "</title></head><body><h1>" << errorCode << " - " << errorMessage << "</h1></body></html>";
	return o.str();
}
