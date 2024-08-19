#include "Impl/ShutdownManagerImpl.h"
#include "IrrigationApplication.h"
#include "Logger/Logger.h"
#include <cstdlib>


ShutdownManagerImpl::ShutdownManagerImpl() :
	task(Task::None)
{
}

ShutdownManagerImpl::~ShutdownManagerImpl() {
	switch (task) {
	case Task::Poweroff:
		execute("poweroff");
		break;
	case Task::Reboot:
		execute("reboot");
		break;
	default:
		break;
	}
}

void ShutdownManagerImpl::powerOff() {
	task = Task::Poweroff;
	IrrigationApplication::getInstance().exit();
}

void ShutdownManagerImpl::reboot() {
	task = Task::Reboot;
	IrrigationApplication::getInstance().exit();
}

void ShutdownManagerImpl::execute(const std::string& command) {
#ifdef __arm__
	LOGGER.debug("Executing command: %s", command.c_str());

	const int systemExitCode = std::system(command.c_str());
	if (0 != systemExitCode) {
		const int processExitCode = (systemExitCode & 0xFF00) >> 8;
		LOGGER.warning("Can not execute command. Error: %d", processExitCode);
	}
#else
	LOGGER.debug("Executing command: %s - Skipped on PC", command.c_str());
#endif
}
