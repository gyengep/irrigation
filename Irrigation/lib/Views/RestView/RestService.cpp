#include "RestService.h"
#include "RestServiceException.h"
#include "XmlErrorWriter.h"
#include "Logger/Logger.h"
#include "WebServer/HttpResponse.h"
#include <algorithm>
#include <sstream>
#include <stdexcept>

using namespace std;


RestService::RestService() :
	errorWriter(new XmlErrorWriter())
{
}

RestService::~RestService() {
}

unique_ptr<HttpResponse> RestService::onRequest(const HttpRequest& request) {
	try {
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
					checkContentType(request);
					checkAccept(request);
					return restServiceCallback->operator() (request, parameters);
				}
			}
		}

		if (pathFound) {
			throw RestMethodNotAllowed(errorWriter, request.getMethod());
		} else {
			throw RestNotFound(errorWriter, request.getUrl());
		}
	} catch (const WebServerException& e) {
		throw;
	} catch (const exception& e) {
		throw RestInternalServerError(errorWriter, e.what());
	}
}

void RestService::checkAccept(const HttpRequest& request) {
	const auto it = request.getHeaders().find("Accept");

	if (request.getHeaders().end() != it) {
		if (it->second != "application/xml" && it->second != "*/*") {
			LOGGER.debug("Not acceptable: %s", it->second.c_str());
			throw RestNotAcceptable(errorWriter, request.getUrl());
		}
	}
}

void RestService::checkContentType(const HttpRequest& request) {
	const auto it = request.getHeaders().find("Content-Type");

	if (request.getHeaders().end() != it) {
		if (it->second != "application/xml") {
			LOGGER.debug("Unsupported media type: %s", it->second.c_str());
			throw RestUnsupportedMediaType(errorWriter, request.getUrl());
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
