#include <iostream>
#include <signal.h>
#include <stdexcept>
#include <sys/stat.h>
#include <unistd.h>
#include "Logger/Logger.h"
#include "Model/IrrigationApplication.h"
#include "Model/Configuration.h"

using namespace std;


void signal_handler(int signo) {
	IrrigationApplication::getInstance().exit();
}

bool initLogger(bool logToCout) {
	try {
		LOGGER.setLevel(Configuration::getInstance().getLogLevel());
		if (logToCout) {
			LOGGER.setOutputStream(cout);
		} else {
			LOGGER.setFileName(Configuration::getInstance().getLogFileName());
		}
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

int main(int argc, char* argv[]) {

	bool logToCout = false;

	if (argc > 1) {
		if ((2 == argc) && (string("--version") == argv[1])) {
			cout << "Irrigation System " << IrrigationApplication::getInstance().getVersion() << endl;
			return EXIT_SUCCESS;
		} else if ((2 == argc) && (string("--stdout") == argv[1])) {
			logToCout = true;
		} else {
			cerr << "Usage: " << argv[0] << " [--version] [--stdout]" << endl;
			exit(EXIT_FAILURE);
		}
	}

	if (!initLogger(logToCout)) {
		exit(EXIT_FAILURE);
	}

	try {
		if (signal(SIGTERM, signal_handler) == SIG_ERR) {
			throw runtime_error("Can't catch SIGTERM");
		}

		if (signal(SIGINT, signal_handler) == SIG_ERR) {
			throw runtime_error("Can't catch SIGINT");
		}

		IrrigationApplication::getInstance().run();
		return EXIT_SUCCESS;

	} catch (const exception& e) {
		LOGGER.error("Irrigation System stopped because of an exception", e);
	} catch(...) {
		LOGGER.error("Irrigation System stopped because of an unknown error");
	}

	return EXIT_FAILURE;
}
