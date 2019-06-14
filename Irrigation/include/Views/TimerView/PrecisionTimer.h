#pragma once
#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>



class PrecisionTimerCallback {
public:
	virtual ~PrecisionTimerCallback() = default;
	virtual void onTimer() = 0;
};

///////////////////////////////////////////////////////////////////////////////

class PrecisionTimer {
	std::mutex terminatedMutex;
	std::mutex threadMutex;
	std::unique_ptr<std::thread> worker;
	std::condition_variable condition;
	PrecisionTimerCallback& callback;
	bool isTerminated;

	const std::chrono::seconds period;
	const std::chrono::milliseconds maxTardiness;

	void workerFunc();
	bool waitForTerminateOrTimeout(const std::chrono::steady_clock::time_point& scheduledExecutionTime);

	bool checkPeriod(const std::chrono::steady_clock::time_point& scheduledExecutionTime);

public:
	PrecisionTimer(PrecisionTimerCallback& callback, const std::chrono::seconds& period);
	~PrecisionTimer();

	void start();
	void stop();
};
