#pragma once

#include <memory>

#include "Command/CommandExecutor.h"
#include "DocumentView/View.h"
#include "Model/IrrigationDocument.h"
#include "Views/WebServerView/TemplateEngine.h"


class IrrigationWebServer;


class WebServerView: public View {
	std::unique_ptr<TemplateEngine> templateEngine;
	std::unique_ptr<CommandExecutor> commandExecutor;
	std::unique_ptr<IrrigationWebServer> webServer;

public:
	WebServerView();
	virtual ~WebServerView();

	virtual void init();
	virtual void terminate();

	IrrigationDocument* getDocument();
};
