#include "Timer.h"
#include "ChronoTools.h"
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <thread>
#include "Logger/Logger.h"


using namespace std;

Timer::Timer(TimerCallback& callback, const chrono::seconds& deltaT) :
	callback(callback),
	isTerminated(false),
	deltaT(deltaT)
{
}

Timer::~Timer() {
	if (nullptr != worker) {
		isTerminated = true;
		terminatedMutex.unlock();

		condition.notify_all();
		worker->join();
	}
}

void Timer::start() {
	// don't actually take the locks yet
	unique_lock<mutex> lock1(terminatedMutex, defer_lock);
	unique_lock<mutex> lock2(threadMutex, defer_lock);

	// lock both unique_locks without deadlock
	lock(lock1, lock2);

	if (nullptr != worker) {
		throw logic_error("Timer::start()  nullptr != worker");
	}

	isTerminated = false;
	terminatedMutex.unlock();

	worker.reset(new thread(&Timer::workerFunc, this));

	LOGGER.debug("1sec timer started");
}

void Timer::stop() {
	// don't actually take the locks yet
	unique_lock<mutex> lock1(terminatedMutex, defer_lock);
	unique_lock<mutex> lock2(threadMutex, defer_lock);

	// lock both unique_locks without deadlock
	lock(lock1, lock2);

	if (nullptr == worker) {
		throw logic_error("Timer::stop()  nullptr == worker");
	}

	isTerminated = true;
	terminatedMutex.unlock();

	condition.notify_all();
	worker->join();
	worker.reset();

	LOGGER.debug("1sec timer stopped");
}

bool Timer::waitForTerminateOrTimeout(const chrono::steady_clock::time_point& wakeupTime) {
	unique_lock<mutex> lock(terminatedMutex);
	return condition.wait_until(lock, wakeupTime, [this]() { return isTerminated; });
}

bool Timer::checkDeltaT(const chrono::steady_clock::time_point& expectedWakeupTime) {
	const chrono::milliseconds maxDiff = chrono::milliseconds(100);
	const chrono::steady_clock::time_point currentTime = chrono::steady_clock::now();
	const chrono::milliseconds actualDiff = chrono::duration_cast<chrono::milliseconds>(currentTime - expectedWakeupTime);

	if (maxDiff < abs(actualDiff)) {
		ostringstream o;
		o << "Update period failure! ";

		if (chrono::seconds(1) < abs(actualDiff)) {
			TimeConverter timeConverter(actualDiff);

			o << timeConverter.getDays() << " days " <<
				timeConverter.getHours() << " hours " <<
				timeConverter.getMinutes() << " minutes " <<
				timeConverter.getSeconds() << " seconds";
		} else {
			o << actualDiff.count() << " ms";
		}

		LOGGER.warning(o.str().c_str());
		return false;
	}

	return true;
}

bool Timer::checkSystemTime(const chrono::system_clock::time_point& systemTime, const chrono::milliseconds& expectedDiff) {

	chrono::milliseconds actualDiff = getDiffBetweenSystemClockAndSteadyClock();
	chrono::milliseconds diffOfDiff = actualDiff - expectedDiff;

	if (abs(diffOfDiff) > chrono::milliseconds(100)) {
		ostringstream o;
		o << "Time is changed! ";

		if (abs(diffOfDiff) > chrono::seconds(1)) {
			time_t previousTime = chrono::system_clock::to_time_t(systemTime);
			time_t currentTime = chrono::system_clock::to_time_t(chrono::system_clock::now());

			o << "from " << put_time(localtime(&previousTime), "%Y.%m.%d %H:%M:%S") << " ";
			o << "to " << put_time(localtime(&currentTime), "%Y.%m.%d %H:%M:%S");
		} else {
			o << "different is: " << diffOfDiff.count() << " ms";
		}

		LOGGER.warning(o.str().c_str());
		return false;
	}

	return true;
}

void Timer::workerFunc() {
	chrono::steady_clock::time_point wakeupTime = chrono::steady_clock::now();

	systemTime = chrono::system_clock::now();
	expectedDiff = getDiffBetweenSystemClockAndSteadyClock();

	while (!waitForTerminateOrTimeout(wakeupTime)) {

		if (!checkDeltaT(wakeupTime)) {
			wakeupTime = chrono::steady_clock::now();
		}

		if (!checkSystemTime(systemTime, expectedDiff)) {
			systemTime = chrono::system_clock::now();
			expectedDiff = getDiffBetweenSystemClockAndSteadyClock();
		}

		callback.onTimer(chrono::system_clock::to_time_t(systemTime));

		wakeupTime += deltaT;
		systemTime += deltaT;
	}
}

