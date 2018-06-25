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
		if (signal(SIGTERM, signal_handler) == SIG_ERR) {
			throw runtime_error("Can't catch SIGTERM");
		}

		Application::getInstance().init();
		Application::getInstance().start();
		Application::getInstance().stop();
		return EXIT_SUCCESS;

	} catch (const exception& e) {
		LOGGER.error("Irrigation system is stopped because of an exception", e);
	} catch(...) {
		LOGGER.error("Irrigation system is stopped because of an unknown error");
	}

	return EXIT_FAILURE;
}
