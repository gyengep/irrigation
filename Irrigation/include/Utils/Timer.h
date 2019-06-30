#pragma once
#include <chrono>
#include <condition_variable>
#include <list>
#include <mutex>
#include <thread>


class TimerCallback {
public:
	virtual ~TimerCallback() = default;
	virtual void onTimer() = 0;
};


class Timer {
public:
	enum class ScheduleType {
		FIXED_RATE,
		FIXED_DELAY
	};

	enum class Priority {
		NORMAL,
		HIGH
	};

private:
	const ScheduleType scheduleType;
	const std::chrono::milliseconds period;
	const std::chrono::milliseconds maxTardiness;

	mutable std::mutex mtx;
	std::condition_variable condition;
	std::thread workerThread;
	std::chrono::steady_clock::time_point nextScheduleTime;
	bool terminated;
	bool changed;

	std::list<TimerCallback*> callbacks;

	void workerFunc();
	bool checkPeriod(const std::chrono::steady_clock::time_point& nextScheduleTime);

public:
	Timer(const std::chrono::milliseconds& period, ScheduleType scheduleType);
	Timer(TimerCallback* callback, const std::chrono::milliseconds& period, ScheduleType scheduleType);
	virtual ~Timer();

	void add(TimerCallback* callback);
	void remove(TimerCallback* callback);
	void removeAll();

	void start(Priority priority = Priority::NORMAL);
	void stop();
};
