#include "WebServer.h"
#include "HttpResponse.h"
#include "HttpRequest.h"
#include "WebServerException.h"
#include "Logger/Logger.h"
#include <algorithm>
#include <cinttypes>
#include <sstream>

using namespace std;


void WebServer::MHD_PanicCallback(void *cls, const char *file, unsigned int line, const char *reason) {
	static_cast<WebServer*>(cls)->panicCallback(file, line, reason);
}

int WebServer::MHD_AccessHandlerCallback(void *cls, struct MHD_Connection *connection,
		const char *url, const char *method, const char *version,
		const char *upload_data, size_t *upload_data_size, void **con_cls) {
	return static_cast<WebServer*>(cls)->accessHandlerCallback(connection,
			url, method, version, upload_data, upload_data_size, con_cls);
}

void WebServer::MHD_RequestCompletedCallback(void *cls, struct MHD_Connection *connection, void **con_cls, enum MHD_RequestTerminationCode toe) {
	static_cast<WebServer*>(cls)->requestCompletedCallback(connection, con_cls, toe);
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
		throw runtime_error("Can not create webserver");
	}

	LOGGER.info("WebServer is started on port: %" PRIu16, port);
}

void WebServer::stop() {
	daemon.reset();

	if (!uploadDatas.empty()) {
		logic_error("WebServer::stop() !datas.empty()");
	}

	LOGGER.info("WebServer is stopped");
}

void WebServer::panicCallback(const char *file, unsigned int line, const char *reason) {
	LOGGER.error("WebServer is stopped working (%s:%u)! reason: %s", file, line, (reason ? reason : "<null>"));
}

void WebServer::requestCompletedCallback(struct MHD_Connection *connection, void **con_cls, enum MHD_RequestTerminationCode toe) {
	auto it = uploadDatas.find(*con_cls);

	if (uploadDatas.end() == it) {
		throw logic_error("WebServer::requestCompletedCallback() datas.end() == it");
	}

	uploadDatas.erase(it);
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

//	if (LOGGER.isLoggable(LogLevel::TRACE)) {
//		ostringstream oss;
//		oss << "HTTP request received: " << method << " " << url << " " << version << endl;
//		oss << "\tconnection: " << connection << endl;
//		oss << "\tupload data ptr: " << (upload_data ? upload_data : "<nullptr>") << endl;
//		oss << "\tupload data size: " << *upload_data_size;
//		LOGGER.trace(oss.str().c_str());
//	} else {
//		LOGGER.debug("HTTP request received: %s %s", method, url);
//	}

	if (NULL == context) {
		/* do never respond on first call */
		shared_ptr<ByteBuffer> connectionUploadData(new ByteBuffer());
		context = connectionUploadData.get();
		uploadDatas.insert(make_pair(context, connectionUploadData));
//		LOGGER.trace("creating upload data: %p", connectionUploadData.get());
		return MHD_YES;
	}

	auto it = uploadDatas.find(context);
	if (uploadDatas.end() == it) {
		throw logic_error("WebServer::accessHandlerCallback() datas.end() == it");
	}

	shared_ptr<ByteBuffer> connectionUploadData = it->second;

	if (0 < *upload_data_size) {
//		LOGGER.trace("concatenate upload data");
		copy_n(upload_data, *upload_data_size, back_inserter(*connectionUploadData));
		*upload_data_size = 0;
		return MHD_YES;
	}

	LOGGER.debug("HTTP request received: %s %s %s", method, url, version);

	unique_ptr<HttpResponse> response;

	try {
		const HttpRequest request(connection, version, method, url, connectionUploadData);
		response = webService->onRequest(request);

		if (response.get() == nullptr) {
			throw runtime_error("HTTP response is NULL");
		}

	} catch (const WebServerException& e) {
		response = HttpResponse::Builder().
				setStatus(e.getStatusCode(), e.getStatusMessage()).
				setBody(e.getErrorMessage()).
				addHeaders(e.getHeaders()).
				build();
	} catch (const exception& e) {
		LOGGER.warning("WebServer caught an unhandled expection", e);
		return MHD_NO;
	}

	return sendResponse(connection, response);
}

int WebServer::sendResponse(struct MHD_Connection* connection, const unique_ptr<HttpResponse>& httpResponse) {
//	if (LOGGER.isLoggable(LogLevel::TRACE)) {
//		ostringstream oss;
//		oss << "Sending HTTP response" << endl;
//		oss << "\tmessage: \"" << httpResponse->getMessage() << "\"" << endl;
//		oss << "\tstatus code: " << httpResponse->getStatusCode();
//		for (const auto& header : httpResponse->gerHeaders()) {
//			oss << endl << "\theader: \"" << header.first << "\": \"" << header.second << "\"";
//		}

//		LOGGER.trace(oss.str().c_str());
//	} else {
		LOGGER.debug("Sending HTTP response: %u %s", httpResponse->getStatusCode(), httpResponse->getStatusMessage().c_str());
//	}

	unique_ptr<MHD_Response, void(*)(struct MHD_Response*)> mhd_response(
		MHD_create_response_from_buffer(httpResponse->getBody().length(), const_cast<char*>(httpResponse->getBody().data()), MHD_RESPMEM_MUST_COPY),
		MHD_destroy_response
		);

	for (const auto& header : httpResponse->gerHeaders()) {
		MHD_add_response_header(mhd_response.get(), header.first.c_str(), header.second.c_str());
	}

	return MHD_queue_response(connection, httpResponse->getStatusCode(), mhd_response.get());
}
