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


int main() {

	LOGGER.setFile(NULL);
	LOGGER.setLevel(Logger::INFO);
	
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
