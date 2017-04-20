#include "common.h"
#include "IrrigationWebServer.h"
#include "TemplateEngine.h"

#include <sstream>
#include <string.h>

#include "WebServer/Response.h"
#include "WebServer/Request.h"
#include "WebServer/WebserverException.h"


IrrigationWebServer::IrrigationWebServer(const std::string& rootDirectory, uint16_t port, TemplateEngine& templateEngine) :
	WebServer(port),
	rootDirectory(rootDirectory),
	templateEngine(templateEngine)
{
}

IrrigationWebServer::~IrrigationWebServer() {
}

Response* IrrigationWebServer::onRequest(const Request& request) {

	if (request.getVersion() != MHD_HTTP_VERSION_1_1) {
		throw WebServerException(MHD_HTTP_HTTP_VERSION_NOT_SUPPORTED);
	}

	if (request.getMethod() != MHD_HTTP_METHOD_GET &&
		request.getMethod() != MHD_HTTP_METHOD_POST) {
		throw WebServerException(MHD_HTTP_NOT_IMPLEMENTED);
	}

	std::string relativePath(rootDirectory + request.getUrl());
	LOGGER.trace("relativePath: %s", relativePath.c_str());

	std::string absolutePath = pathHandler.createAbsolutePath(relativePath);
	LOGGER.trace("absolutePath: %s", absolutePath.c_str());

	if (absolutePath.empty()) {
		throw WebServerException(MHD_HTTP_NOT_FOUND);
	}

	std::ifstream ifs(absolutePath, std::ios::binary);

	if (!ifs.is_open()) {
		throw WebServerException(MHD_HTTP_NOT_FOUND);
	}

	std::stringstream fileContent;
	fileContent << ifs.rdbuf();

	const std::string& result = templateEngine.generate(fileContent.str(), request.getParameters());
	return new Response(request, result);

	//std::string fileContent;
	//fileContent.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
	//return new Response(request, fileContent);
}
