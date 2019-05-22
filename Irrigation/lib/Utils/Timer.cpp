#include "Timer.h"
#include "Logger/Logger.h"

using namespace std;


Timer::Timer(TimerCallback& callback, const std::chrono::milliseconds& milliseconds) :
	waitTime(milliseconds),
	callback(callback),
	terminated(false)
{
}

Timer::Timer(TimerCallback& callback, const chrono::seconds& seconds) :
	Timer(callback, chrono::duration_cast<chrono::milliseconds>(seconds))
{}

Timer::~Timer() {
}

void Timer::start() {
	workerThread = thread(&Timer::workerFunc, this);
}

void Timer::stop() {
	{
		unique_lock<mutex> lock(mtx);
		terminated = true;
	}

	condition.notify_all();
	workerThread.join();
}

bool Timer::waitForTerminateOrTimeout() {
	unique_lock<mutex> lock(mtx);
	return condition.wait_for(lock, waitTime, [this]() { return terminated; });
}

void Timer::workerFunc() {
	try {
		while (!waitForTerminateOrTimeout()) {
			callback.onTimer();
		}
	} catch (const exception& e) {
		LOGGER.warning("Unhandled exception is caught in timer thread", e);
	}
}
