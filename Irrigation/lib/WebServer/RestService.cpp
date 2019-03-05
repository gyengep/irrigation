#include "RestService.h"
#include "HttpResponse.h"
#include "WebServerException.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;


RestService::RestService() {
}

RestService::~RestService() {
}

unique_ptr<HttpResponse> RestService::onRequest(const HttpRequest& request) {
	Path requestPath;
	PathTemplate::split(request.getUrl(), requestPath);

	bool pathFound = false;

	PathInfos::const_iterator it;
	for (it = pathInfos.begin(); pathInfos.end() != it; ++it) {

		const PathInfo& pathInfo = it->second;
		KeyValue parameters;

		if (pathInfo.path->evaluate(requestPath, parameters)) {
			pathFound = true;

			const RestServiceCallback* restServiceCallback = findCallback(pathInfo.callbacks, request.getMethod());
			if (restServiceCallback) {
				return restServiceCallback->operator() (request, parameters);
			}
		}
	}

	if (pathFound) {
		throw HttpMethodNotAllowed(request.getMethod());
	} else {
		throw HttpNotFount(request.getUrl());
	}
}

const RestServiceCallback* RestService::findCallback(const Callbacks& callbacks, const string& method) {
	Callbacks::const_iterator it = callbacks.find(method);

	if (callbacks.end() == it) {
		return NULL;
	}

	return &(it->second);
}

void RestService::addPath(const string& method, const string& path, RestServiceCallback restServiceCallback) {
	Callbacks& callbacks = getCallbacksForPath(path);
	callbacks.insert(make_pair(method, restServiceCallback));
}

RestService::Callbacks& RestService::getCallbacksForPath(const string& path) {
	PathInfos::iterator it = pathInfos.find(path);

	if (pathInfos.end() == it) {
		it = pathInfos.insert(make_pair(path, PathInfo(new PathTemplate(path)))).first;
	}

	return it->second.callbacks;
}
