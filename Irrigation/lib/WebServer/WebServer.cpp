#include "HttpResponse.h"
#include "HttpRequest.h"
#include "WebServer.h"
#include "WebServerException.h"
#include "Logger/Logger.h"

#include <algorithm>
#include <cstring>
#include <functional>
#include <inttypes.h>
#include <sstream>
#include <vector>

using namespace std;


std::mutex WebServer::mtx;
std::map<void*, WebServer*> WebServer::webServers;


WebServer* WebServer::getWebserver(void *cls) {
	lock_guard<mutex> lock(mtx);

	auto it = webServers.find(cls);
	if (webServers.end() == it) {
		return NULL;
	}
	return it->second;
}

bool WebServer::addWebserver(WebServer* webServer) {
	lock_guard<mutex> lock(mtx);

	auto it = webServers.find(webServer);
	if (webServers.end() != it) {
		return false;
	}

	webServers.insert(make_pair(webServer, webServer));
	return true;
}

bool WebServer::deleteWebserver(void *cls) {
	lock_guard<mutex> lock(mtx);

	auto it = webServers.find(cls);
	if (webServers.end() == it) {
		return false;
	}

	webServers.erase(it);
	return true;
}

void WebServer::MHD_PanicCallback(void *cls, const char *file, unsigned int line, const char *reason) {
	WebServer* webServer = getWebserver(cls);

	if (NULL == webServer) 	{
		logic_error("WebServer::MHD_PanicCallback() webserver doesn't exist");
	}

	return webServer->panicCallback(file, line, reason);
}

void WebServer::MHD_RequestCompletedCallback(void *cls, struct MHD_Connection *connection, void **con_cls, enum MHD_RequestTerminationCode toe) {
	WebServer* webServer = getWebserver(cls);

	if (NULL == webServer) 	{
		logic_error("WebServer::MHD_RequestCompletedCallback() webserver doesn't exist");
	}

	return webServer->requestCompletedCallback(connection, con_cls, toe);
}

int WebServer::MHD_AccessHandlerCallback(
		void *cls,
		struct MHD_Connection* connection,
		const char* url,
		const char* method,
		const char* version,
		const char* upload_data,
		size_t* upload_data_size,
		void** con_cls) {

	WebServer* webServer = getWebserver(cls);

	if (NULL == webServer) 	{
		logic_error("WebServer::MHD_PanicCallback() webserver doesn't exist");
	}

	return webServer->accessHandlerCallback(connection,
			url, method, version,
			upload_data, upload_data_size,
			con_cls);
}

///////////////////////////////////////////////////////////////////////////////

WebServer::WebServer(shared_ptr<WebService> webService, uint16_t port) :
	webService(webService),
	port(port),
	daemon(nullptr, MHD_stop_daemon)
{
}

WebServer::~WebServer() {
}

void WebServer::start() {
	if (!addWebserver(this)) {
		throw logic_error("WebServer::WebServer() webServer already exist with same this pointer");
	}

	MHD_set_panic_func(MHD_PanicCallback, this);

	daemon.reset(MHD_start_daemon(
			MHD_USE_INTERNAL_POLLING_THREAD, port,
			NULL, NULL,
			MHD_AccessHandlerCallback, this,
			MHD_OPTION_CONNECTION_LIMIT, 10,
			MHD_OPTION_CONNECTION_TIMEOUT, 10,
			MHD_OPTION_NOTIFY_COMPLETED, MHD_RequestCompletedCallback, this,
			MHD_OPTION_END));


	if (daemon == nullptr) {
		deleteWebserver(this);
		throw runtime_error("Can not create webserver");
	}

	LOGGER.info("WebServer is started on port: %" PRIu16 , port);
}

void WebServer::stop() {
	daemon.reset();

	if (!datas.empty()) {
		logic_error("WebServer::stop() !datas.empty()");
	}

	if (!deleteWebserver(this)) {
		logic_error("WebServer::stop() webServer doesn't exist with current this pointer");
	}

	LOGGER.info("WebServer is stopped");
}

void WebServer::panicCallback(const char *file, unsigned int line, const char *reason) {
	LOGGER.error("WebServer is stopped working (%s:%u)! reason: %s", file, line, (reason ? reason : "<null>"));
}

void WebServer::requestCompletedCallback(struct MHD_Connection *connection, void **con_cls, enum MHD_RequestTerminationCode toe) {
	auto it = datas.find(*con_cls);

	if (datas.end() == it) {
		throw logic_error("WebServer::requestCompletedCallback() datas.end() == it");
	}

	datas.erase(it);
}

int WebServer::accessHandlerCallback(
		struct MHD_Connection* connection,
		const char* url,
		const char* method,
		const char* version,
		const char* upload_data,
		size_t* upload_data_size,
		void** con_cls) {

	void*& context = *con_cls;

	if (LOGGER.isLoggable(LogLevel::TRACE)) {
		ostringstream oss;
		oss << "HTTP request received: " << method << " " << url << " " << version << endl;
		oss << "\tconnection: " << connection << endl;
		oss << "\tupload data ptr: " << (upload_data ? upload_data : "<nullptr>") << endl;
		oss << "\tupload data size: " << *upload_data_size;
		LOGGER.trace(oss.str().c_str());
	} else {
		LOGGER.debug("HTTP request received: %s %s", method, url);
	}

	if (NULL == context) {
		/* do never respond on first call */
		shared_ptr<ByteBuffer> connectionUploadData(new ByteBuffer());
		context = connectionUploadData.get();
		datas.insert(make_pair(context, connectionUploadData));
		LOGGER.trace("creating upload data: %p", connectionUploadData.get());
		return MHD_YES;
	}

	auto it = datas.find(context);
	if (datas.end() == it) {
		throw logic_error("WebServer::accessHandlerCallback() datas.end() == it");
	}

	shared_ptr<ByteBuffer> connectionUploadData = it->second;

	if (0 < *upload_data_size) {
		LOGGER.trace("concatenate upload data");
		copy_n(upload_data, *upload_data_size, back_inserter(*connectionUploadData));
		*upload_data_size = 0;
		return MHD_YES;
	}

	try {
		HttpRequest request(connection, version, method, url, connectionUploadData);
		unique_ptr<HttpResponse> response = webService->onRequest(request);

		if (response.get() == nullptr) {
			throw std::runtime_error("HTTP response is NULL");
		}

		return sendResponse(connection, response);
	} catch (const exception& e) {
		LOGGER.warning("WebServer caught an expection: ", e);
	}

	return MHD_NO;
}

int WebServer::sendResponse(struct MHD_Connection* connection, const std::unique_ptr<HttpResponse>& httpResponse) {
	if (LOGGER.isLoggable(LogLevel::TRACE)) {
		ostringstream oss;
		oss << "Sending HTTP response" << endl;
		oss << "\tmessage: \"" << httpResponse->getMessage() << "\"" << endl;
		oss << "\tstatus code: " << httpResponse->getStatusCode();
		for (const auto& header : httpResponse->gerHeaders()) {
			oss << endl << "\theader: \"" << header.first << "\": \"" << header.second << "\"";
		}

		LOGGER.trace(oss.str().c_str());
	} else {
		LOGGER.debug("sending HTTP response. status code: %u", httpResponse->getStatusCode());
	}

	unique_ptr<MHD_Response, void(*)(struct MHD_Response*)> mhd_response(
		MHD_create_response_from_buffer(httpResponse->getMessage().length(), const_cast<char*>(httpResponse->getMessage().data()), MHD_RESPMEM_MUST_COPY),
		MHD_destroy_response
		);

	for (const auto& header : httpResponse->gerHeaders()) {
		MHD_add_response_header(mhd_response.get(), header.first.c_str(), header.second.c_str());
	}

	return MHD_queue_response(connection, httpResponse->getStatusCode(), mhd_response.get());
}
