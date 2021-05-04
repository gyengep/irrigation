#pragma once
#include <chrono>


class WaitObserver {
public:
	virtual ~WaitObserver() = default;
	virtual void wait_for(const std::chrono::milliseconds& ms) = 0;
	virtual void wait_for_pred(const std::chrono::milliseconds& ms) = 0;
	virtual void wait() = 0;
	virtual void wait_pred() = 0;
};
