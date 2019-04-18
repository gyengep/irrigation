#pragma once
#include <functional>
#include <map>
#include <memory>
#include "WebServer/KeyValue.h"
#include "WebServer/HttpRequest.h"
#include "WebServer/HttpResponse.h"
#include "WebServer/WebServer.h"
#include "ErrorWriter.h"
#include "PathTemplate.h"

typedef std::function<std::unique_ptr<HttpResponse>(const HttpRequest& request, const KeyValue& pathParameters)> RestServiceCallback;


class RestService : public WebService {

	typedef std::map<const std::string, RestServiceCallback> Callbacks;

	struct PathInfo {
		std::unique_ptr<PathTemplate> path;
		Callbacks callbacks;

		PathInfo(PathTemplate* path) : path(path) {}
	};

	typedef std::map<const std::string, PathInfo> PathInfos;
	typedef std::map<const std::string, std::shared_ptr<ErrorWriter>> ErrorWriters;

	PathInfos pathInfos;
	std::shared_ptr<ErrorWriter> errorWriter;

	void checkContentType(const HttpRequest& request);
	void checkAccept(const HttpRequest& request);
	Callbacks& getCallbacksForPath(const std::string& path);
	static const RestServiceCallback* findCallback(const Callbacks& callbacks, const std::string& method);

public:
	RestService();
	virtual ~RestService();

	void addPath(const std::string& method, const std::string& path, RestServiceCallback restServiceCallback);
	std::shared_ptr<ErrorWriter> getErrorWriter() const { return errorWriter; }

	virtual std::unique_ptr<HttpResponse> onRequest(const HttpRequest& request) override;

	static bool isAcceptable(const std::string& acceptHeader);
};
