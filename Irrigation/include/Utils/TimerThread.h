#pragma once
#include <chrono>
#include <condition_variable>
#include <mutex>
#include "Thread.h"


class TimerThread : public Thread {
	mutable std::mutex mtx;

	std::condition_variable cv;
	std::chrono::milliseconds delay;
	bool stopped;

	virtual void onExecute() override;
	virtual void onTimer() = 0;

public:
	TimerThread(const std::string& name = "");
	virtual ~TimerThread();

	void schedule(const std::chrono::milliseconds& delay);
	void stop();
};
