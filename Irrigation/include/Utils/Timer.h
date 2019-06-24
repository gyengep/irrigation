#pragma once
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <set>
#include <thread>


class TimerCallback {
public:
	virtual ~TimerCallback() = default;
	virtual void onTimer() = 0;
};

class Timer {
	enum class ScheduleType {
		FIXED_RATE,
		FIXED_DELAY
	};

	struct CallbackProperties {
		TimerCallback* const callback;
		const ScheduleType scheduleType;
		const std::chrono::milliseconds waitTime;
		std::chrono::steady_clock::time_point nextScheduleTime;

		CallbackProperties(TimerCallback* const callback, const ScheduleType scheduleType, const std::chrono::milliseconds waitTime);
		bool operator<(const CallbackProperties& other) const;
	};

	mutable std::mutex mtx;
	std::condition_variable condition;
	std::thread workerThread;
	bool terminated;
	bool changed;

	std::multiset<CallbackProperties> callbacks;

	std::multiset<CallbackProperties>::const_iterator find(TimerCallback* const callback) const;

	void start();
	void stop();
	void workerFunc();

public:
	Timer();
	virtual ~Timer();

	void scheduleFixedRate(TimerCallback* callback, const std::chrono::milliseconds& waitTime);
	void scheduleFixedDelay(TimerCallback* callback, const std::chrono::milliseconds& waitTime);
	void remove(TimerCallback* const callback);
};
