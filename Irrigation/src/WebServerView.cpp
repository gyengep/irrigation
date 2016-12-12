/*
 * WebServerView.cpp
 *
 *  Created on: 2016. nov. 15.
 *      Author: Rendszergazda
 */

#include "Common.h"
#include "WebServerView.h"

WebServerView::WebServerView(Document* document) :
	View(document),
	isTerminated(false),
	webServer("./html/", 80, document)
{
	workerThread = new std::thread(&IrrigationWebServer::doService, &webServer);
}

WebServerView::~WebServerView() {
	isTerminated = true;

	workerThread->join();
	delete workerThread;
	workerThread = NULL;
}

