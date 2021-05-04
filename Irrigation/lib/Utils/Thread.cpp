#include "Thread.h"
#include "Logger/Logger.h"

using namespace std;


Thread::Thread(const string& name) :
	runnable(),
	name(name)
{
}

Thread::Thread(const std::shared_ptr<Runnable>& runnable, const string& name) :
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

Runnable& Thread::getRunnable() {
	Runnable* runnablePtr;

	if (runnable) {
		runnablePtr = runnable.get();
	} else {
		runnablePtr = this;
	}

	return *runnablePtr;
}

void Thread::run() {
}

void Thread::onRun() {
	getRunnable().run();
}

void Thread::onStop() {
	getRunnable().interrupt();
}

void Thread::start() {
	if (workerThread.joinable()) {
		throw logic_error("Thread::start() workerThread.joinable()");
	}

	workerThread = thread(&Thread::workerFunction, this);
}

void Thread::stop() {
	if (!workerThread.joinable()) {
		throw logic_error("Thread::stop() !workerThread.joinable()");
	}

	onStop();
	workerThread.join();
}

void Thread::workerFunction() {
	LOGGER.debug("Thread [%s] is started", name.c_str());

	try {
		onRun();
	} catch (const exception& e) {
		LOGGER.warning(string("Unhandled exception is caught in Thread [" + name + "]").c_str(), e);
	}

	LOGGER.debug("Thread [%s] is finished", name.c_str());
}

const string& Thread::getName() const {
	return name;
}
