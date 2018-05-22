#include <iostream>
#include "Logger/Logger.h"
#include "Model/IrrigationApplication.h"

using namespace std;


int main() {
	LOGGER.setOutput(cout);
	LOGGER.setLevel(Logger::TRACE);
	
	try {
		Application::getInstance().run();
	} catch(exception& e) {
		LOGGER.error(e.what());
	}

	return 0;
}
