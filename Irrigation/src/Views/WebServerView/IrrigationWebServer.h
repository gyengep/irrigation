#pragma once

#include "Utils/PathHandler.h"
#include <WebServer/WebServer.h>

class TemplateEngine;


class IrrigationWebServer: public WebServer {
	const std::string rootDirectory;
	TemplateEngine& templateEngine;
	CommandExecutor& commandExecutor;
	PathHandler pathHandler;

public:
	IrrigationWebServer(const std::string& rootDirectory, uint16_t port, TemplateEngine& templateEngine, CommandExecutor& commandExecutor);
	virtual ~IrrigationWebServer();

	virtual Response* onRequest(const Request& request);
};
