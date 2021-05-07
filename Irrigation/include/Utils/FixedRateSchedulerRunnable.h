#pragma once
#include <chrono>
#include <memory>
#include "SynchronizationObject.h"
#include "Thread.h"


class FixedRateSchedulerRunnable : public Runnable {
	const std::shared_ptr<Runnable> runnable;
	const std::chrono::milliseconds period;
	const std::string name;
	SynchronizationObject synchronizationObject;

	bool interrupted;

	std::chrono::system_clock::time_point nextScheduleTime;

	bool checkWaitTime(const std::chrono::milliseconds& waitTime) const;

public:
	FixedRateSchedulerRunnable(const std::shared_ptr<Runnable>& runnable, const std::chrono::milliseconds& period, const std::string& name);
	virtual ~FixedRateSchedulerRunnable();

	void setSynchronizationObjectName(const std::string& syncName);

	virtual void run() override;
	virtual void interrupt() override;

	static std::string periodText(const std::chrono::milliseconds& t);
};
