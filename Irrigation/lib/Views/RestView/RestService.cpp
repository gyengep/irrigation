#include "RestService.h"
#include "RestServiceException.h"
#include "XmlErrorWriter.h"
#include "Logger/Logger.h"
#include "WebServer/HttpResponse.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;


RestService::RestService() :
	errorWriterFactory(new XmlErrorWriterFactory())
{
}

RestService::~RestService() {
}

unique_ptr<HttpResponse> RestService::onRequest(const HttpRequest& request) {
	Path requestPath;
	PathTemplate::split(request.getUrl(), requestPath);

	bool pathFound = false;

	for (auto it = pathInfos.begin(); pathInfos.end() != it; ++it) {

		const PathInfo& pathInfo = it->second;
		KeyValue parameters;

		if (pathInfo.path->evaluate(requestPath, parameters)) {
			pathFound = true;

			const RestServiceCallback* restServiceCallback = findCallback(pathInfo.callbacks, request.getMethod());
			if (restServiceCallback) {
				checkAccept(request.getHeaders());
				return restServiceCallback->operator() (request, parameters);
			}
		}
	}

	checkAccept(request.getHeaders());

	if (pathFound) {
		throw RestHttpMethodNotAllowed(errorWriterFactory->create(), request.getMethod(), "application/xml");
	} else {
		throw RestHttpNotFound(errorWriterFactory->create(), request.getUrl(), "application/xml");
	}
}

void RestService::checkAccept(const KeyValue& headers) {
	const auto it = headers.find("Accept");

	if (headers.end() != it) {
		if (it->second != "application/xml" && it->second != "*/*") {
			throw HttpNotAcceptable();
		}
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
