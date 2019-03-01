#pragma once
#include <map>
#include <memory>
#include <vector>
#include <microhttpd.h>

class HttpResponse;
class HttpRequest;


class WebService {
public:
	virtual ~WebService() = default;
	virtual std::unique_ptr<HttpResponse> onRequest(const HttpRequest& request) = 0;
};

class WebServer {
	typedef std::vector<char> ByteBuffer;

	const std::shared_ptr<WebService> webService;
	const uint16_t port;

	std::map<void*, std::shared_ptr<ByteBuffer>> uploadDatas;
	std::unique_ptr<MHD_Daemon, void(*)(struct MHD_Daemon*)> daemon;

	int accessHandlerCallback(struct MHD_Connection *connection,
		const char *url, const char *method, const char *version,
		const char *upload_data, size_t *upload_data_size, void **con_cls);
	void panicCallback(const char *file, unsigned int line, const char *reason);
	void requestCompletedCallback(struct MHD_Connection *connection, void **con_cls, enum MHD_RequestTerminationCode toe);
	int sendResponse(struct MHD_Connection* connection, const std::unique_ptr<HttpResponse>& httpResponse);

	static void MHD_PanicCallback(void *cls, const char *file, unsigned int line, const char *reason);
	static int MHD_AccessHandlerCallback(void *cls, struct MHD_Connection *connection,
			const char *url, const char *method, const char *version,
			const char *upload_data, size_t *upload_data_size, void **con_cls);
	static void MHD_RequestCompletedCallback(void *cls, struct MHD_Connection *connection, void **con_cls, enum MHD_RequestTerminationCode toe);

public:
	WebServer(std::shared_ptr<WebService> webService, uint16_t port);
	virtual ~WebServer();

	void start();
	void stop();
};
