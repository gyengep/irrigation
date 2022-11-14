#include "Thread.h"
#include "Logger/Logger.h"


Thread::Thread(const std::shared_ptr<Runnable>& runnable, const std::string& name) :
	runnable(runnable),
	name(name)
{
	if (nullptr == runnable) {
		throw std::logic_error("Thread::Thread() runnable can not be null");
	}
}

Thread::~Thread() {
	if (workerThread.joinable()) {
		LOGGER.warning("Thread [%s] is not stopped", name.c_str());
	}
}

void Thread::start() {
	if (workerThread.joinable()) {
		throw std::logic_error("Thread::start() workerThread.joinable()");
	}

	workerThread = std::thread(&Thread::workerFunction, this);
}

void Thread::stop() {
	if (!workerThread.joinable()) {
		throw std::logic_error("Thread::stop() !workerThread.joinable()");
	}

	runnable->interrupt();
	workerThread.join();
}

void Thread::workerFunction() {
	LOGGER.debug("Thread [%s] is started", name.c_str());

	try {
		runnable->run();
	} catch (const std::exception& e) {
		LOGGER.warning(std::string("Unhandled exception is caught in Thread [" + name + "]").c_str(), e);
	}

	LOGGER.debug("Thread [%s] is finished", name.c_str());
}
