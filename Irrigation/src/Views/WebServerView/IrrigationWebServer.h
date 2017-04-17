#pragma once

#include "Utils/PathHandler.h"
#include "WebServer/WebServer.h"

class TemplateEngine;


class IrrigationWebServer: public WebServer {
	const std::string rootDirectory;
	TemplateEngine& templateEngine;
	PathHandler pathHandler;

public:
	IrrigationWebServer(const std::string& rootDirectory, uint16_t port, TemplateEngine& templateEngine);
	virtual ~IrrigationWebServer();

	virtual Response* onRequest(const Request& request);
};
