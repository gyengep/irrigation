#include "RestView.h"
#include "Logger/Logger.h"
#include "Model/IrrigationDocument.h"
#include "WebServer/RestService.h"

using namespace std;


RestView::RestView(IrrigationDocument& irrigationDocument, uint16_t port) :
	View(irrigationDocument),
	port(port)
{
	restService.reset(new RestService());
	webServer.reset(new WebServer(restService, port));
}

RestView::~RestView() {
}

void RestView::initialize() {
	LOGGER.debug("RestView initializing...");

	webServer->start();

	LOGGER.debug("RestView initialized");
}

void RestView::terminate() {
	LOGGER.debug("RestView terminating...");

	webServer->stop();

	LOGGER.debug("RestView terminated");
}

