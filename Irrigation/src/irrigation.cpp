#include <iostream>
#include <signal.h>
#include <stdexcept>
#include "Logger/Logger.h"
#include "Model/IrrigationApplication.h"

using namespace std;


void signal_handler(int signo) {
	Application::getInstance().terminate();
}

int main() {
	
	try {
		//LOGGER.setOutput(cout);
		LOGGER.setLevel(Logger::TRACE);

		if (signal(SIGTERM, signal_handler) == SIG_ERR) {
			throw runtime_error("Can't catch SIGTERM");
		}

		Application::getInstance().run();
	} catch(exception& e) {
		LOGGER.error(e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
