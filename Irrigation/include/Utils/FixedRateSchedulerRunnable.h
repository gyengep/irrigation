#pragma once
#include <chrono>
#include <memory>
#include "SynchronizationObject.h"
#include "Thread.h"

/*
class FixedRateScheduler : public Thread {
	const std::chrono::milliseconds period;
	SynchronizationObject synchronizationObject;

	bool stopped;

	void wait(const std::chrono::milliseconds& waitTime);

	virtual void onRun() override;
	virtual void onStop() override;

public:
	FixedRateScheduler(const std::shared_ptr<Runnable>& runnable, const std::chrono::milliseconds& period, const std::string& name);
	virtual ~FixedRateScheduler();
};
*/
