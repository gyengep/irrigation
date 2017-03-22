/*
 * WebServerView.cpp
 *
 *  Created on: 2016.12.20.
 *      Author: 502664609
 */

#include "Common.h"
#include "WebServerView.h"
#include "IrrigationWebServer.h"


class Document;


WebServerView::WebServerView() {
	webServer = new IrrigationWebServer("", 80, NULL);
}

WebServerView::~WebServerView() {
	delete webServer;
}

