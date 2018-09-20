#pragma once
#include <condition_variable>
#include <memory>
#include <mutex>



class TimerCallback {
public:
	virtual ~TimerCallback() = default;
	virtual void onTimer(time_t rawTime) = 0;
};

///////////////////////////////////////////////////////////////////////////////

class Timer {
	std::mutex terminatedMutex;
	std::mutex threadMutex;
	std::unique_ptr<std::thread> worker;
	std::condition_variable condition;
	TimerCallback& callback;
	bool isTerminated;

	std::chrono::milliseconds expectedDiff;
	std::chrono::system_clock::time_point systemTime;

	const std::chrono::seconds deltaT;

	void workerFunc();
	bool waitForTerminateOrTimeout(const std::chrono::steady_clock::time_point& wakeupTime);

	static bool checkDeltaT(const std::chrono::steady_clock::time_point& wakeupTime);
	static bool checkSystemTime(const std::chrono::system_clock::time_point& systemTime, const std::chrono::milliseconds& expectedDiff);

public:
	Timer(TimerCallback& callback, const std::chrono::seconds& deltaT);
	~Timer();

	void start();
	void stop();
};
