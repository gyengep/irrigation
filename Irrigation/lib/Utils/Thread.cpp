#include "Thread.h"
#include "Logger/Logger.h"
#include <cstring>

using namespace std;


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

void Thread::start() {
	start(Priority::NORMAL);
}

void Thread::start(Priority priority) {
	if (workerThread.joinable()) {
		throw logic_error("Thread::start() workerThread.joinable()");
	}

	workerThread = thread(&Thread::workerFunction, this);

	if (Priority::HIGH == priority) {
		struct sched_param param;
		std::memset(&param, 0, sizeof(param));

		param.sched_priority = 1;
		const int result = pthread_setschedparam(workerThread.native_handle(), SCHED_RR, &param);
		if (0 != result) {
			LOGGER.warning("Can not change thread [%s] priority. (error code: %d)", name.c_str(), result);
		}
	}
}

void Thread::stop() {
	if (!workerThread.joinable()) {
		throw logic_error("Thread::stop() !workerThread.joinable()");
	}

	runnable->interrupt();
	workerThread.join();
}

void Thread::workerFunction() {
	LOGGER.debug("Thread [%s] is started", name.c_str());

	try {
		runnable->run();
	} catch (const exception& e) {
		LOGGER.warning(string("Unhandled exception is caught in Thread [" + name + "]").c_str(), e);
	}

	LOGGER.debug("Thread [%s] is finished", name.c_str());
}
