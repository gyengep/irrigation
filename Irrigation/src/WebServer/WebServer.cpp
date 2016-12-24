/*
 * WebServer.cpp
 *
 *  Created on: 2016. nov. 15.
 *      Author: Rendszergazda
 */

#include "Common.h"
#include "Answer.h"
#include "Request.h"
#include "WebServer.h"
#include "WebServerException.h"

#include <cstring>


std::set<WebServer*> WebServer::instances;


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
			WebServer::answer_to_connection, this,
			MHD_OPTION_ARRAY, ops, MHD_OPTION_END);

	if (NULL == daemon) {
		instances.erase(this);
		throw WebServerException("Can not create webserver");
	}
}

WebServer::~WebServer() {
	MHD_stop_daemon(daemon);

	auto it = instances.find(this);
	if (instances.end() != it) {
		instances.erase(it);
	} else {
		LOGGER.warning("WebServer already deleted: 0x%08X", this);
	}
}

void WebServer::send(const Answer& answer) {

	struct MHD_Response *response = MHD_create_response_from_buffer(answer.getDataLength(), (void*)answer.getData(), MHD_RESPMEM_MUST_COPY);
	int ret = MHD_queue_response(static_cast<MHD_Connection*>(answer.getConnection()), answer.getStatusCode(), response);
	MHD_destroy_response(response);

	if (MHD_YES != ret) {
		throw WebServerException("Can not create response");
	}
}

int WebServer::answer_to_connection(void *cls,
		struct MHD_Connection* connection, const char* url, const char* method, const char* version,
		const char* upload_data, size_t* upload_data_size, void** con_cls) {

	if (NULL == cls) {
		LOGGER.warning("WebServer callback must not be NULL _1_");
		return MHD_NO;
	}

	WebServer* webServer = static_cast<WebServer*>(cls);

	if (WebServer::instances.end() == WebServer::instances.find(webServer)) {
		LOGGER.warning("Invalid WebServer callback");
		return MHD_NO;
	}

	try {
		Request request(connection, url, method, version, upload_data, upload_data_size);
		webServer->onRequest(request);

	} catch (WebServerException& e) {
		LOGGER.warning(e.what());
		return MHD_NO;
	}

	return MHD_YES;
}
