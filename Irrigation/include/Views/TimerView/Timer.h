#pragma once
#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>



class TimerCallback {
public:
	virtual ~TimerCallback() = default;
	virtual void onTimer() = 0;
};

///////////////////////////////////////////////////////////////////////////////

class Timer {
	std::mutex terminatedMutex;
	std::mutex threadMutex;
	std::unique_ptr<std::thread> worker;
	std::condition_variable condition;
	TimerCallback& callback;
	bool isTerminated;

	const std::chrono::seconds deltaT;

	void workerFunc();
	bool waitForTerminateOrTimeout(const std::chrono::steady_clock::time_point& wakeupTime);

	static bool checkDeltaT(const std::chrono::steady_clock::time_point& wakeupTime);

public:
	Timer(TimerCallback& callback, const std::chrono::seconds& deltaT);
	~Timer();

	void start();
	void stop();
};
