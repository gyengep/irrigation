#include "Timer.h"
#include "ChronoTools.h"
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <pthread.h>

using namespace std;


Timer::Timer(const chrono::milliseconds& period, ScheduleType scheduleType, const string& name) :
	scheduleType(scheduleType),
	period(period),
	maxTardiness(period),
	name(name),
	terminated(false),
	changed(false)
{
}

Timer::~Timer() {
	if (workerThread.joinable()) {
		LOGGER.error("Timer thread [%s] is not stopped", name.c_str());
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
			LOGGER.warning("Can not change thread [%s] priority. (error code: %d)", name.c_str(), result);
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

	if (LOGGER.isLoggable(LogLevel::DEBUG)) {
		string periodText;
		if (period < chrono::seconds(1)) {
			periodText = to_string(chrono::duration_cast<chrono::duration<double>>(period).count()) + " seconds";
		} else if (period < chrono::hours(1)) {
				periodText = to_string(chrono::duration_cast<chrono::seconds>(period).count()) + " seconds";
		} else {
			periodText = to_string(chrono::duration_cast<chrono::hours>(period).count()) + " hours";
		}

		LOGGER.debug("Timer thread [%s] is started with period: %s", name.c_str(), periodText.c_str());
	}

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
				for (const auto& callback : callbacks) {
					callback->onTimer();
				}

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
			}
		}
	} catch (const exception& e) {
		LOGGER.warning(string("Unhandled exception is caught in timer thread [" + name + "]").c_str(), e);
	}

	LOGGER.debug("Timer thread [%s] is stopped", name.c_str());
}

bool Timer::checkPeriod(const chrono::steady_clock::time_point& nextScheduleTime) {
	const chrono::milliseconds actualDiff = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - nextScheduleTime);

	if (abs(actualDiff) > maxTardiness) {

		if (LOGGER.isLoggable(LogLevel::WARNING)) {
			TimeConverter timeConverter(actualDiff);
			ostringstream o;

			o << "Update period failure in timer thread [" << name << "]! ";
			o << timeConverter.getDays() << " days ";
			o << setw(2) << setfill('0') << timeConverter.getHours() << ":";
			o << setw(2) << setfill('0') << timeConverter.getMinutes() << ":";
			o << setw(2) << setfill('0') << timeConverter.getSeconds() << ".";
			o << setw(3) << setfill('0') << timeConverter.getMillis();

			LOGGER.warning(o.str().c_str());
		}
		return false;
	}

	return true;
}
