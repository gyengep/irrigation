//============================================================================
// Name        : Irrigation.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "common.h"
#include "Application.h"

#ifndef IRRIGATION_TEST

#include "WebServer.h"

int main() {
	Application application;
	
	IrrigationWebServer server("c:/Temp/html/", 80);
	server.doService();

	try {
		application.init();
		application.run();
		application.uninit();
	} catch(std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	
	return 0;
}

#endif
