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
			throw HTTP_405_Method_Not_Allowed(errorWriter, request.getMethod());
		} else {
			throw HTTP_404_Not_Found(errorWriter, request.getUrl());
		}
	} catch (const WebServerException& e) {
		throw;
	} catch (const exception& e) {
		throw HTTP_500_Internal_Server_Error(errorWriter, e.what());
	}
}

void RestService::checkAccept(const HttpRequest& request) {
	const auto it = request.getHeaders().find("Accept");

	if (request.getHeaders().end() != it) {

		if (!isAcceptable(it->second)) {
			LOGGER.debug("Not acceptable: %s", it->second.c_str());
			throw HTTP_406_Not_Acceptable(errorWriter, request.getUrl());
		}
	}
}

bool RestService::isAcceptable(const string& acceptHeader) {
	size_t posBegin = 0, posEnd;

	while ((posEnd = acceptHeader.find_first_of(",;", posBegin)) != string::npos) {
		size_t trimmedPosBegin = acceptHeader.find_first_not_of(' ', posBegin);
		size_t trimmedPosEnd = acceptHeader.find_last_not_of(' ', posEnd - 1);
		const string subItem = acceptHeader.substr(trimmedPosBegin, trimmedPosEnd - trimmedPosBegin + 1);

		if (subItem == "*/*" || subItem == "application/xml") {
			return true;
		}

		posBegin = posEnd + 1;
	}

	size_t trimmedPosBegin = acceptHeader.find_first_not_of(' ', posBegin);
	size_t trimmedPosEnd = acceptHeader.find_last_not_of(" ,;", posEnd - 1);
	string subItem;
	if (trimmedPosEnd == string::npos) {
		subItem = acceptHeader.substr(trimmedPosBegin);
	} else {
		subItem = acceptHeader.substr(trimmedPosBegin, trimmedPosEnd - trimmedPosBegin + 1);
	}

	if (subItem == "*/*" || subItem == "application/xml") {
		return true;
	}

	return false;
}

void RestService::checkContentType(const HttpRequest& request) {
	const auto it = request.getHeaders().find("Content-Type");

	if (request.getHeaders().end() != it) {
		if (it->second != "application/xml") {
			LOGGER.debug("Unsupported media type: %s", it->second.c_str());
			throw HTTP_415_Unsupported_Media_Type(errorWriter, request.getUrl());
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
