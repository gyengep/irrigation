#pragma once
#include <functional>
#include <map>
#include <memory>
#include "PathTemplate.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "WebServer.h"

typedef std::function<std::unique_ptr<HttpResponse>(const HttpRequest& request, const Parameters& pathParameters)> RestServiceCallback;


class RestService : public WebService {

	typedef std::map<const std::string, RestServiceCallback> Callbacks;

	struct PathInfo {
		std::unique_ptr<PathTemplate> path;
		Callbacks callbacks;

		PathInfo(PathTemplate* path) : path(path) {}
	};

	typedef std::map<const std::string, PathInfo> PathInfos;

	PathInfos pathInfos;

	Callbacks& getCallbacksForPath(const std::string& path);
	static const RestServiceCallback* findCallback(const Callbacks& callbacks, const std::string& method);

public:
	RestService();
	virtual ~RestService();

	void addPath(const std::string& method, const std::string& path, RestServiceCallback restServiceCallback);

	virtual std::unique_ptr<HttpResponse> onRequest(const HttpRequest& request) override;
};
