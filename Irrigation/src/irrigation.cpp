#include <signal.h>
#include <stdexcept>
#include <sys/stat.h>
#include <unistd.h>
#include "Logger/Logger.h"
#include "Model/IrrigationApplication.h"
#include "Model/Configuration.h"

using namespace std;



void signal_handler(int signo) {
	Application::getInstance().terminate();
}

bool initLogger() {
	try {
		LOGGER.setLevel(Configuration::getInstance().getLogLevel());
		LOGGER.setFileName(Configuration::getInstance().getLogFileName());
	} catch (const exception& e) {
		return false;
	}

	return true;
}

//void daemonize() {
//	/* Our process ID and Session ID */
//	pid_t pid, sid;
//
//	/* Fork off the parent process */
//	pid = fork();
//	if (pid < 0) {
//		exit(EXIT_FAILURE);
//	}
//
//	/* If we got a good PID, then we can exit the parent process. */
//	if (pid > 0) {
//		exit(EXIT_SUCCESS);
//	}
//
//	/* Change the file mode mask */
//	umask(S_IWGRP | S_IWOTH);
//
//	/* Open any logs here */
//	if (!initLogger()) {
//		exit(EXIT_FAILURE);
//	}
//
//	/* Create a new SID for the child process */
//	sid = setsid();
//	if (sid < 0) {
//		LOGGER.error("Can not create SID");
//		exit(EXIT_FAILURE);
//	}
//
//	/* Change the current working directory */
//	if ((chdir("/")) < 0) {
//		LOGGER.error("Can not change working directory");
//		exit(EXIT_FAILURE);
//	}
//
//	/* Close out the standard file descriptors */
//	close(STDIN_FILENO);
//	close(STDOUT_FILENO);
//	close(STDERR_FILENO);
//}

int main() {
	if (!initLogger()) {
		exit(EXIT_FAILURE);
	}

	try {
		if (signal(SIGTERM, signal_handler) == SIG_ERR) {
			throw runtime_error("Can't catch SIGTERM");
		}

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
