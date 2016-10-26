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
/*
#include <teng.h>
// include writer
#include <tengwriter.h>
// include errorlog
#include <tengerror.h>

using namespace Teng;
*/
int main() {

	LOGGER.setFile(NULL);
	LOGGER.setLevel(Logger::INFO);

	// create Teng engine
	//Teng_t teng("", Teng_t::LM_LOG_TO_OUTPUT | Teng_t::LM_ERROR_FRAGMENT);

	
	//IrrigationWebServer server("c:/Temp/html/", 80);
	//server.doService();

	Application application;

	try {
		application.init();
		application.run();
		application.uninit();
	} catch(std::exception& e) {
		LOGGER.error(e.what());
	}

	return 0;
}

#endif
