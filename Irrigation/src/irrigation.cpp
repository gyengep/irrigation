#include <iostream>
#include <signal.h>
#include <stdexcept>
#include "Logger/Logger.h"
#include "Model/IrrigationApplication.h"
#include "Model/Exceptions/InvalidConfigFileException.h"
#include "Model/Exceptions/ConfigFileOpenException.h"

using namespace std;

enum ExitCode {
	SUCCESS,
	UNKNOWN_ERROR,
	UNKNOWN_EXCEPTION,
	CANT_OPEN_CONFIG_FILE,
	INVALID_CONFIG_FILE
};


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
		return ExitCode::SUCCESS;

	} catch (const ConfigFileOpenException& e) {
		LOGGER.error(e.what());
		return ExitCode::CANT_OPEN_CONFIG_FILE;
	} catch (const InvalidConfigFileException& e) {
		LOGGER.error(e.what());
		return ExitCode::INVALID_CONFIG_FILE;
	} catch(exception& e) {
		LOGGER.error("Unknown exception: %s", e.what());
		return UNKNOWN_EXCEPTION;
	} catch(...) {
		LOGGER.error("Unknown error!");
		return UNKNOWN_ERROR;
	}
}
