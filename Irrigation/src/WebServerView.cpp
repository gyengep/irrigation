/*
 * WebServerView.cpp
 *
 *  Created on: 2016.11.10.
 *      Author: 502664609
 */

#include "common.h"
#include "WebServerView.h"

#include <stdio.h>


WebServerView::WebServerView(Document* document) :
	View(document),
	webServer("./html/", 80, document)
{
	workerThread = new std::thread(&IrrigationWebServer::doService, &webServer);
}

WebServerView::~WebServerView() {
}

