//============================================================================
// Name        : Irrigation.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "common.h"
#include <exception>
#include "Application.h"

int main() {
	Application application;
	
	try {
		application.init();
		application.run();
		application.uninit();
	} catch(std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	
	return 0;
}
