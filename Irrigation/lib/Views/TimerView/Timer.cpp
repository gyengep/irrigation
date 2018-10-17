#include "Timer.h"
#include "ChronoTools.h"
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <thread>
#include "Logger/Logger.h"

using namespace std;
using namespace std::chrono;


Timer::Timer(TimerCallback& callback, const seconds& deltaT) :
	callback(callback),
	isTerminated(false),
	deltaT(deltaT)
{
}

Timer::~Timer() {
	if (nullptr != worker) {
		LOGGER.warning("Timer::~Timer(): worker thread is running");

		unique_lock<mutex> lock1(terminatedMutex, defer_lock);
		unique_lock<mutex> lock2(threadMutex, defer_lock);

		lock(lock1, lock2);

		isTerminated = true;
		lock1.unlock();

		condition.notify_all();
		worker->join();
	}
}

void Timer::start() {
	unique_lock<mutex> lock1(terminatedMutex, defer_lock);
	unique_lock<mutex> lock2(threadMutex, defer_lock);

	lock(lock1, lock2);

	if (nullptr != worker) {
		throw logic_error("Timer::start()  nullptr != worker");
	}

	isTerminated = false;
	lock1.unlock();

	worker.reset(new thread(&Timer::workerFunc, this));

	LOGGER.debug("%ld sec timer started", (long int)deltaT.count());
}

void Timer::stop() {
	unique_lock<mutex> lock1(terminatedMutex, defer_lock);
	unique_lock<mutex> lock2(threadMutex, defer_lock);

	lock(lock1, lock2);

	if (nullptr == worker) {
		throw logic_error("Timer::stop()  nullptr == worker");
	}

	isTerminated = true;
	lock1.unlock();

	condition.notify_all();
	worker->join();
	worker.reset();

	LOGGER.debug("%ld sec timer stopped", (long int)deltaT.count());
}

bool Timer::waitForTerminateOrTimeout(const steady_clock::time_point& wakeupTime) {
	unique_lock<mutex> lock(terminatedMutex);
	return condition.wait_until(lock, wakeupTime, [this]() { return isTerminated; });
}

bool Timer::checkDeltaT(const steady_clock::time_point& expectedWakeupTime) {
	const steady_clock::time_point currentTime = steady_clock::now();
	const milliseconds actualDiff = duration_cast<milliseconds>(currentTime - expectedWakeupTime);

	if (actualDiff < milliseconds(-1000) || milliseconds(1000) < actualDiff) {
		TimeConverter timeConverter(actualDiff);
		ostringstream o;

		o << "Update period failure! ";
		o << timeConverter.getDays() << " days ";
		o << setw(2) << setfill('0') << timeConverter.getHours() << ":";
		o << setw(2) << setfill('0') << timeConverter.getMinutes() << ":";
		o << setw(2) << setfill('0') << timeConverter.getSeconds() << ".";
		o << setw(3) << setfill('0') << timeConverter.getMillis();

		LOGGER.warning(o.str().c_str());
		return false;
	}

	return true;
}

void Timer::workerFunc() {
	steady_clock::time_point wakeupTime = steady_clock::now();

	while (!waitForTerminateOrTimeout(wakeupTime)) {

		if (!checkDeltaT(wakeupTime)) {
			wakeupTime = steady_clock::now();
		}

		callback.onTimer();
		wakeupTime += deltaT;
	}
}

