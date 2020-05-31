#include "Thread.h"
#include "Logger/Logger.h"

using namespace std;


Thread::Thread(const string& name) :
	name(name)
{
}

Thread::~Thread() {
}

void Thread::start() {
	workerThread = thread(&Thread::workerFunction, this);
}

void Thread::join() {
	if (!workerThread.joinable()) {
		throw logic_error("Thread::join() !workerThread.joinable()");
	}

	workerThread.join();
}

void Thread::workerFunction() {
	LOGGER.debug("Thread [%s] is started", name.c_str());

	try {
		onExecute();
	} catch (const exception& e) {
		LOGGER.warning(string("Unhandled exception is caught in Thread [" + name + "]").c_str(), e);
	}

	LOGGER.debug("Thread [%s] is finished", name.c_str());
}

const string& Thread::getName() const {
	return name;
}
