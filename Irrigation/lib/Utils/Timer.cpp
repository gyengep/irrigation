#include "Timer.h"
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"
#include <algorithm>
//#include <pthread.h>

using namespace std;


Timer::CallbackProperties::CallbackProperties(TimerCallback* const callback, const ScheduleType scheduleType, const std::chrono::milliseconds waitTime) :
	callback(callback),
	scheduleType(scheduleType),
	waitTime(waitTime),
	nextScheduleTime(chrono::steady_clock::now() + waitTime)
{
}

bool Timer::CallbackProperties::operator<(const CallbackProperties& other) const {
	return (nextScheduleTime < other.nextScheduleTime);
}

///////////////////////////////////////////////////////////////////////////////

Timer::Timer() :
	terminated(false),
	changed(false)
{
	start();
	//pthread_setschedparam(workerThread.native_handle(), policy, {priority});
}

Timer::~Timer() {
	stop();
}

void Timer::start() {
	workerThread = thread(&Timer::workerFunc, this);
}

void Timer::stop() {
	unique_lock<mutex> lock(mtx);

	terminated = true;
	lock.unlock();
	condition.notify_all();
	workerThread.join();
}

std::multiset<Timer::CallbackProperties>::const_iterator Timer::find(TimerCallback* const callback) const {
	auto predicate = [callback](const CallbackProperties& properties) {
		return properties.callback == callback;
	};

	return find_if(callbacks.begin(), callbacks.end(), predicate);
}

void Timer::scheduleFixedRate(TimerCallback* callback, const std::chrono::milliseconds& waitTime) {
	unique_lock<mutex> lock(mtx);

	if (callbacks.end() != find(callback)) {
		throw logic_error("Timer::scheduleFixedRate() callbacks.end() != find(callback)");
	}

	callbacks.insert(CallbackProperties(callback, ScheduleType::FIXED_RATE, waitTime));
	changed = true;
	condition.notify_all();
}

void Timer::scheduleFixedDelay(TimerCallback* callback, const std::chrono::milliseconds& waitTime) {
	unique_lock<mutex> lock(mtx);

	if (callbacks.end() != find(callback)) {
		throw logic_error("Timer::scheduleFixedDelay() callbacks.end() != find(callback)");
	}

	callbacks.insert(CallbackProperties(callback, ScheduleType::FIXED_DELAY, waitTime));
	changed = true;
	condition.notify_all();
}

void Timer::remove(TimerCallback* const callback) {
	unique_lock<mutex> lock(mtx);

	const auto it = find(callback);
	if (callbacks.end() == it) {
		throw logic_error("Timer::remove() callbacks.end() == it");
	}

	callbacks.erase(it);
	changed = true;
	condition.notify_all();
}

void Timer::cancel() {
	unique_lock<mutex> lock(mtx);

	callbacks.clear();
	changed = true;
	condition.notify_all();
}

void Timer::workerFunc() {

	auto terminatedOrChanged = [this]() {
		return (terminated || changed);
	};

	try {
		unique_lock<mutex> lock(mtx);

		while (!terminated) {
			changed = false;

			if (callbacks.empty()) {
				condition.wait(lock, terminatedOrChanged);
			} else {
				condition.wait_until(lock, callbacks.begin()->nextScheduleTime, terminatedOrChanged);
			}

			if (!terminated && !changed) {
				CallbackProperties callbackProperties = *callbacks.begin();
				callbackProperties.callback->onTimer();

				callbacks.erase(callbacks.begin());

				switch (callbackProperties.scheduleType) {
				case ScheduleType::FIXED_RATE:
					callbackProperties.nextScheduleTime += callbackProperties.waitTime;
					break;
				case ScheduleType::FIXED_DELAY:
					callbackProperties.nextScheduleTime = chrono::steady_clock::now() + callbackProperties.waitTime;
					break;
				}

				callbacks.insert(callbackProperties);
			}
		}
	} catch (const exception& e) {
		LOGGER.warning("Unhandled exception is caught in timer thread", e);
	}
}
