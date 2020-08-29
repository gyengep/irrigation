#pragma once
#include <chrono>
#include <vector>
#include "InterruptableWait.h"


class IncrementalWait {
	const std::vector<std::chrono::milliseconds> waitTimes;

	InterruptableWait interruptableWait;
	size_t waitTimeIdx;

public:
	IncrementalWait(const std::vector<std::chrono::milliseconds>& waitTimes);

	void resetWaitTime();
	void incrementWaitTime();
	void wait();
	void interrupt();

	std::chrono::milliseconds getWaitTime() const;
};
