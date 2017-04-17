#include "common.h"
#include "WebServerView.h"
#include "IrrigationWebServer.h"


WebServerView::WebServerView() {
}

WebServerView::~WebServerView() {
}

IrrigationDocument* WebServerView::getDocument() {
	return dynamic_cast<IrrigationDocument*>(View::getDocument());
}

void WebServerView::init() {
	LOGGER.debug("WebServerView starting");
	templateEngine.reset(new TemplateEngine(getDocument()));
	webServer.reset(new IrrigationWebServer("./html/", 80, *templateEngine));
	LOGGER.debug("WebServerView started");
}

void WebServerView::terminate() {
	LOGGER.debug("WebServerView stopping");
	webServer.reset(nullptr);
	LOGGER.debug("WebServerView stopped");
}
