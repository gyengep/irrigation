#pragma once
#include <chrono>
#include <condition_variable>
#include <mutex>


class InterruptableWait {
	std::mutex mtx;
	std::condition_variable cv;
	bool finished;

public:
	InterruptableWait();

	void wait_for(const std::chrono::milliseconds& ms);
	void finish();
};
