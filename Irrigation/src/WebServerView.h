/*
 * WebServerView.h
 *
 *  Created on: 2016.11.10.
 *      Author: 502664609
 */

#ifndef WEBSERVERVIEW_H_
#define WEBSERVERVIEW_H_

#include <atomic>
#include <thread>

#include "View.h"
#include "WebServer.h"

class WebServerView: public View {
	std::thread* workerThread;
	std::atomic_bool isTerminated;
	IrrigationWebServer webServer;

public:
	WebServerView(Document* document);
	virtual ~WebServerView();
};

#endif /* WEBSERVERVIEW_H_ */
