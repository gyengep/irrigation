#pragma once
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>


class TimerCallback {
public:
	virtual ~TimerCallback() = default;
	virtual void onTimer() = 0;
};

class Timer {
	const std::chrono::seconds waitTime;
	TimerCallback& callback;

	mutable std::mutex mtx;
	std::condition_variable condition;
	bool terminated;

	std::thread workerThread;

	bool waitForTerminateOrTimeout();
	void workerFunc();

public:
	Timer(TimerCallback& callback, const std::chrono::seconds& seconds, bool startAutomatically = false);
	virtual ~Timer();

	void start();
	void stop();
};
