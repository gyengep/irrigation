#include "Application.h"
#include "Logger/Logger.h"


Application::Application() :
	isTerminated(false)
{
}

Application::~Application() {
}

void Application::run() {
	onInitialize();

	LOGGER.debug("Main loop started");

	std::unique_lock<std::mutex> lock(mtx);
	while (!isTerminated) {
		condition.wait(lock);
	}

	LOGGER.debug("Main loop finished");

	onTerminate();
}

void Application::exit() {
	LOGGER.debug("Terminate called");

	std::unique_lock<std::mutex> lock(mtx);
	isTerminated = true;
	lock.unlock();
	condition.notify_all();
}

void Application::onInitialize() {
}

void Application::onTerminate() {
}

