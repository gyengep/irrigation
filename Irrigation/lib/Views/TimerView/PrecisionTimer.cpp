#include "PrecisionTimer.h"
#include "ChronoTools.h"
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <thread>
#include "Logger/Logger.h"

using namespace std;
using namespace std::chrono;


PrecisionTimer::PrecisionTimer(PrecisionTimerCallback& callback, const seconds& period) :
	callback(callback),
	isTerminated(false),
	period(period),
	maxTardiness(milliseconds(1000))
{
}

PrecisionTimer::~PrecisionTimer() {
	if (nullptr != worker) {
		LOGGER.warning("PrecisionTimer::~PrecisionTimer(): worker thread is running");

		unique_lock<mutex> lock1(terminatedMutex, defer_lock);
		unique_lock<mutex> lock2(threadMutex, defer_lock);

		lock(lock1, lock2);

		isTerminated = true;
		lock1.unlock();

		condition.notify_all();
		worker->join();
	}
}

void PrecisionTimer::start() {
	unique_lock<mutex> lock1(terminatedMutex, defer_lock);
	unique_lock<mutex> lock2(threadMutex, defer_lock);

	lock(lock1, lock2);

	if (nullptr != worker) {
		throw logic_error("PrecisionTimer::start()  nullptr != worker");
	}

	isTerminated = false;
	lock1.unlock();

	worker.reset(new thread(&PrecisionTimer::workerFunc, this));

	LOGGER.debug("%ld sec timer started", (long int)period.count());
}

void PrecisionTimer::stop() {
	unique_lock<mutex> lock1(terminatedMutex, defer_lock);
	unique_lock<mutex> lock2(threadMutex, defer_lock);

	lock(lock1, lock2);

	if (nullptr == worker) {
		throw logic_error("PrecisionTimer::stop()  nullptr == worker");
	}

	isTerminated = true;
	lock1.unlock();

	condition.notify_all();
	worker->join();
	worker.reset();

	LOGGER.debug("%ld sec timer stopped", (long int)period.count());
}

bool PrecisionTimer::waitForTerminateOrTimeout(const steady_clock::time_point& scheduledExecutionTime) {
	unique_lock<mutex> lock(terminatedMutex);
	return condition.wait_until(lock, scheduledExecutionTime, [this]() { return isTerminated; });
}

bool PrecisionTimer::checkPeriod(const steady_clock::time_point& scheduledExecutionTime) {
	const milliseconds actualDiff = duration_cast<milliseconds>(steady_clock::now() - scheduledExecutionTime);

	if (abs(actualDiff) > maxTardiness) {
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

void PrecisionTimer::workerFunc() {
	steady_clock::time_point scheduledExecutionTime = steady_clock::now();

	while (!waitForTerminateOrTimeout(scheduledExecutionTime)) {
		if (!checkPeriod(scheduledExecutionTime)) {
			scheduledExecutionTime = steady_clock::now();
		}

		callback.onTimer();
		scheduledExecutionTime += period;
	}
}

