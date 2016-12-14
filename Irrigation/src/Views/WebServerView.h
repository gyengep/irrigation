/*
 * WebServerView.h
 *
<<<<<<< HEAD
 *  Created on: 2016.11.10.
 *      Author: 502664609
=======
 *  Created on: 2016. nov. 15.
 *      Author: Rendszergazda
>>>>>>> 3a2eba7f6c3df431d940e1543f50c2bb710afb74
 */

#ifndef WEBSERVERVIEW_H_
#define WEBSERVERVIEW_H_

#include <atomic>
#include <thread>

#include "Model/View.h"
#include "WebServer/WebServer.h"

class WebServerView: public View {
	std::thread* workerThread;
	std::atomic_bool isTerminated;
	//IrrigationWebServer webServer;

public:
	WebServerView(Document* document);
	virtual ~WebServerView();
};

#endif /* WEBSERVERVIEW_H_ */
