#include "Timer.h"
#include "ChronoTools.h"
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <pthread.h>

using namespace std;


Timer::Timer(const chrono::milliseconds& period, ScheduleType scheduleType) :
	scheduleType(scheduleType),
	period(period),
	maxTardiness(period),
	terminated(false),
	changed(false)
{
}

Timer::Timer(TimerCallback* callback, const chrono::milliseconds& period, ScheduleType scheduleType) :
	Timer(period, scheduleType)
{
	add(callback);
}

Timer::~Timer() {
	if (workerThread.joinable()) {
		LOGGER.error("Timer thread is not stopped");
	}
}

void Timer::start(Timer::Priority priority) {
	unique_lock<mutex> lock(mtx);
	nextScheduleTime = chrono::steady_clock::now() + period;
	workerThread = thread(&Timer::workerFunc, this);

	if (Priority::HIGH == priority) {
		struct sched_param param;
		memset(&param, 0, sizeof(param));

		param.sched_priority = 1;
		int result;
		if (0 != (result = pthread_setschedparam(workerThread.native_handle(), SCHED_RR, &param))) {
			LOGGER.warning("Can not change thread priority. (error code: %d)", result);
		}
	}
}

void Timer::stop() {
	unique_lock<mutex> lock(mtx);

	terminated = true;
	lock.unlock();
	condition.notify_all();
	workerThread.join();
}

void Timer::add(TimerCallback* callback) {
	unique_lock<mutex> lock(mtx);

	if (callbacks.end() != find(callbacks.begin(), callbacks.end(), callback)) {
		throw logic_error("Timer::scheduleFixedRate() callbacks.end() != find(callback)");
	}

	callbacks.push_back(callback);
	changed = true;
	lock.unlock();
	condition.notify_all();
}

void Timer::remove(TimerCallback* callback) {
	unique_lock<mutex> lock(mtx);

	const auto it = find(callbacks.begin(), callbacks.end(), callback);
	if (callbacks.end() == it) {
		throw logic_error("Timer::remove() callbacks.end() == it");
	}

	callbacks.erase(it);
	changed = true;
	lock.unlock();
	condition.notify_all();
}

void Timer::removeAll() {
	unique_lock<mutex> lock(mtx);

	callbacks.clear();
	changed = true;
	lock.unlock();
	condition.notify_all();
}

const chrono::milliseconds& Timer::getPeriod() const {
	return period;
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
				condition.wait_until(lock, nextScheduleTime, terminatedOrChanged);
			}

			if (!terminated && !changed) {
				switch (scheduleType) {
				case ScheduleType::FIXED_RATE:

					if (!checkPeriod(nextScheduleTime)) {
						nextScheduleTime = chrono::steady_clock::now();
					}

					nextScheduleTime += period;
					break;
				case ScheduleType::FIXED_DELAY:
					nextScheduleTime = chrono::steady_clock::now() + period;
					break;
				}

				for (const auto& callback : callbacks) {
					callback->onTimer();
				}
			}
		}
	} catch (const exception& e) {
		LOGGER.warning("Unhandled exception is caught in timer thread", e);
	}
}

bool Timer::checkPeriod(const chrono::steady_clock::time_point& nextScheduleTime) {
	const chrono::milliseconds actualDiff = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - nextScheduleTime);

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
