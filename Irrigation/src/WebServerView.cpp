/*
 * WebServerView.cpp
 *
 *  Created on: 2016. nov. 15.
 *      Author: Rendszergazda
 */

#include "WebServerView.h"

WebServerView::WebServerView(Document* document) :
	View(document),
	webServer("./html/", 80),
	isTerminated(false)
{
	workerThread = new std::thread(&IrrigationWebServer::doService, &webServer);
}

WebServerView::~WebServerView() {
	isTerminated = true;

	workerThread->join();
	delete workerThread;
	workerThread = NULL;
}

