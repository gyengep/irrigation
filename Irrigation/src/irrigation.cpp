#include <signal.h>
#include <stdexcept>
#include "Logger/Logger.h"
#include "Model/IrrigationApplication.h"
#include "Model/Configuration.h"

using namespace std;



void signal_handler(int signo) {
	Application::getInstance().terminate();
}

int main() {

	try {
		if (signal(SIGTERM, signal_handler) == SIG_ERR) {
			throw runtime_error("Can't catch SIGTERM");
		}

		LOGGER.setLevel(Configuration::getInstance().getLogLevel());
		LOGGER.setFileName(Configuration::getInstance().getLogFileName());

		Application::getInstance().init();
		Application::getInstance().start();
		Application::getInstance().stop();
		return EXIT_SUCCESS;

	} catch (const exception& e) {
		LOGGER.error("Irrigation System stopped because of an exception", e);
	} catch(...) {
		LOGGER.error("Irrigation System stopped because of an unknown error");
	}

	return EXIT_FAILURE;
}
