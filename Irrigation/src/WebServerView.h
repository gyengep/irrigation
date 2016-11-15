/*
 * WebServerView.h
 *
 *  Created on: 2016. nov. 15.
 *      Author: Rendszergazda
 */

#ifndef WEBSERVERVIEW_H_
#define WEBSERVERVIEW_H_

#include <atomic>
#include <thread>


#include "View.h"
#include "WebServer.h"


class WebServerView : public View {
	std::thread* workerThread;
	std::atomic_bool isTerminated;
	IrrigationWebServer webServer;

	void workerFunc();

public:
	WebServerView(Document* document);
	virtual ~WebServerView();
};

#endif /* WEBSERVERVIEW_H_ */
