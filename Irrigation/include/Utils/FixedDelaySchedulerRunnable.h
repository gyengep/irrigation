#pragma once
#include <chrono>
#include <memory>
#include <string>
#include "Runnable.h"
#include "SynchronizationObject.h"


class FixedDelaySchedulerRunnable : public Runnable {
	const std::shared_ptr<Runnable> runnable;
	const std::chrono::milliseconds firstDelay;
	const std::chrono::milliseconds delay;
	const std::string name;

	SynchronizationObject synchronizationObject;

	bool interrupted;

	void wait(const std::chrono::milliseconds& waitTime);
	virtual bool skipRun();

public:
	FixedDelaySchedulerRunnable(const std::shared_ptr<Runnable>& runnable, const std::chrono::milliseconds& firstDelay, const std::chrono::milliseconds& delay, const std::string& name);
	virtual ~FixedDelaySchedulerRunnable();

	void setSynchronizationObjectName(const std::string& syncName);

	virtual void run() override;
	virtual void interrupt() override;

	static std::string periodText(const std::chrono::milliseconds& t);
};

///////////////////////////////////////////////////////////////////////////////

class EveryHourSchedulerRunnable : public FixedDelaySchedulerRunnable {
	std::tm lastRun;

	virtual bool skipRun() override;

public:
	EveryHourSchedulerRunnable(const std::shared_ptr<Runnable>& runnable, const std::string& name);
	virtual ~EveryHourSchedulerRunnable();
};

///////////////////////////////////////////////////////////////////////////////

class EveryDaySchedulerRunnable : public FixedDelaySchedulerRunnable {
	std::tm lastRun;

	virtual bool skipRun() override;

public:
	EveryDaySchedulerRunnable(const std::shared_ptr<Runnable>& runnable, const std::string& name);
	virtual ~EveryDaySchedulerRunnable();
};
