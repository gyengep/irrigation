#include "Timer.h"

using namespace std;


Timer::Timer(TimerCallback& callback, const std::chrono::seconds& seconds, bool startAutomatically) :
	waitTime(seconds),
	callback(callback),
	terminated(false)
{
	if (startAutomatically) {
		start();
	}
}

Timer::~Timer() {
	stop();
}

void Timer::start() {
	unique_lock<mutex> lock(mtx);
	workerThread = thread(&Timer::workerFunc, this);
}

void Timer::stop() {
	{
		unique_lock<mutex> lock(mtx);
		terminated = true;
	}

	condition.notify_all();

	if (workerThread.joinable()) {
		workerThread.join();
	}
}

bool Timer::waitForTerminateOrTimeout() {
	unique_lock<mutex> lock(mtx);
	return condition.wait_for(lock, waitTime, [this]() { return terminated; });
}

void Timer::workerFunc() {
	while (!waitForTerminateOrTimeout()) {
		callback.onTimer();
	}
}
