#include "common.h"
#include "IrrigationWebServer.h"
#include "TemplateEngine.h"

#include <sstream>
#include <string.h>

#include "WebServer/Response.h"
#include "WebServer/Request.h"
#include "WebServer/WebserverException.h"


IrrigationWebServer::IrrigationWebServer(const std::string& rootDirectory, uint16_t port, TemplateEngine& templateEngine, CommandExecutor& commandExecutor) :
	WebServer(port),
	rootDirectory(rootDirectory),
	templateEngine(templateEngine),
	commandExecutor(commandExecutor)
{
}

IrrigationWebServer::~IrrigationWebServer() {
}

Response* IrrigationWebServer::onRequest(const Request& request) {

	if (0 != strcmp(request.getVersion(), MHD_HTTP_VERSION_1_1)) {
		throw WebServerException(MHD_HTTP_HTTP_VERSION_NOT_SUPPORTED);
	}

	if (0 != strcmp(request.getMethod(), MHD_HTTP_METHOD_GET)) {
		throw WebServerException(MHD_HTTP_NOT_IMPLEMENTED);
	}

	LOGGER.debug("HTTP request received");
	LOGGER.trace("connection: %p", request.getConnection());
	LOGGER.trace("url: %s", request.getUrl());
	LOGGER.trace("method: %s", request.getMethod());
	LOGGER.trace("version: %s", request.getVersion());
	LOGGER.trace("size: %u", *request.getUploadDataSize());

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

	const std::string& result = templateEngine.generate(fileContent.str());
	return new Response(request, result);

	//std::string fileContent;
	//fileContent.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
	//return new Response(request, fileContent);
}
