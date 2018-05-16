#include <iostream>
#include "Logger/Logger.h"
#include "Model/IrrigationApplication.h"


int main() {
	LOGGER.setOutput(std::cout);
	LOGGER.setLevel(Logger::TRACE);
	
	try {
		Application::getInstance().run();
	} catch(std::exception& e) {
		LOGGER.error(e.what());
	}

	return 0;
}
