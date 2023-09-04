#include "WebServer.h"
#include "HttpResponse.h"
#include "HttpRequest.h"
#include "WebServerException.h"
#include "Logger/Logger.h"
#include "Utils/DateTime.h"
#include <algorithm>
#include <cinttypes>
#include <sstream>


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
WebServer::WebServer(const std::shared_ptr<WebService>& webService, uint16_t port, const std::shared_ptr<FileWriterFactory>& accessLogWriterFactory) :
	webService(webService),
	port(port),
	accessLogWriterFactory(accessLogWriterFactory),
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
		throw std::runtime_error("Can not create webserver");
	}

	LOGGER.info("WebServer is started on port: %" PRIu16, port);
}

void WebServer::stop() {
	daemon.reset();

	if (!uploadDatas.empty()) {
		std::logic_error("WebServer::stop() !datas.empty()");
	}

	LOGGER.info("WebServer is stopped");
}

void WebServer::panicCallback(const char *file, unsigned int line, const char *reason) {
	LOGGER.error("WebServer is stopped working (%s:%u)! reason: %s", file, line, (reason ? reason : "<null>"));
}

void WebServer::requestCompletedCallback(struct MHD_Connection *connection, void **con_cls, enum MHD_RequestTerminationCode toe) {
	auto it = uploadDatas.find(*con_cls);

	if (uploadDatas.end() == it) {
		throw std::logic_error("WebServer::requestCompletedCallback() datas.end() == it");
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
		std::shared_ptr<ByteBuffer> connectionUploadData(new ByteBuffer());
		context = connectionUploadData.get();
		uploadDatas.insert(make_pair(context, connectionUploadData));
//		LOGGER.trace("creating upload data: %p", connectionUploadData.get());
		return MHD_YES;
	}

	auto it = uploadDatas.find(context);
	if (uploadDatas.end() == it) {
		throw std::logic_error("WebServer::accessHandlerCallback() datas.end() == it");
	}

	std::shared_ptr<ByteBuffer> connectionUploadData = it->second;

	if (0 < *upload_data_size) {
//		LOGGER.trace("concatenate upload data");
		std::copy_n(upload_data, *upload_data_size, back_inserter(*connectionUploadData));
		*upload_data_size = 0;
		return MHD_YES;
	}

	std::unique_ptr<HttpResponse> response;
	std::unique_ptr<HttpRequest> request;

	try {
		request = std::unique_ptr<HttpRequest>(new HttpRequest(connection, version, method, url, connectionUploadData));
		response = webService->onRequest(*request);

		if (response.get() == nullptr) {
			throw std::runtime_error("HTTP response is NULL");
		}

	} catch (const WebServerException& e) {
		response = HttpResponse::Builder().
				setStatus(e.getStatusCode(), e.getStatusMessage()).
				setBody(e.getErrorMessage()).
				addHeaders(e.getHeaders()).
				build();
	} catch (const std::exception& e) {
		LOGGER.warning("WebServer caught an unhandled expection", e);
		return MHD_NO;
	}

	writeAccessLog(*request, *response);
	return sendResponse(connection, response);
}

void WebServer::writeAccessLog(const HttpRequest& request, const HttpResponse& response) {
	std::ostringstream oss;
	oss << LocalDateTime(DateTime::now()).toString() << " \"" << request.getMethod() << " " << request.getUrl();

	if (false == request.getParametersAsText().empty()) {
		oss << "?" << request.getParametersAsText();
	}

	oss << " " << request.getVersion() << "\" " << response.getStatusCode() << std::endl;
	accessLogWriterFactory->create(FileWriter::Type::APPEND)->write(oss.str());
}

int WebServer::sendResponse(struct MHD_Connection* connection, const std::unique_ptr<HttpResponse>& httpResponse) {
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
//		LOGGER.debug("Sending HTTP response: %u %s", httpResponse->getStatusCode(), httpResponse->getStatusMessage().c_str());
//	}

	std::unique_ptr<MHD_Response, void(*)(struct MHD_Response*)> mhd_response(
			MHD_create_response_from_buffer(httpResponse->getBody().length(), const_cast<char*>(httpResponse->getBody().data()), MHD_RESPMEM_MUST_COPY),
			MHD_destroy_response
		);

	for (const auto& header : httpResponse->gerHeaders()) {
		MHD_add_response_header(mhd_response.get(), header.first.c_str(), header.second.c_str());
	}

	return MHD_queue_response(connection, httpResponse->getStatusCode(), mhd_response.get());
}
