#pragma once
#include <chrono>
#include <memory>
#include "SynchronizationObject.h"
#include "Thread.h"


class FixedDelaySchedulerThread : public Thread {
	const std::chrono::milliseconds firstDelay;
	const std::chrono::milliseconds delay;
	SynchronizationObject synchronizationObject;

	bool stopped;

	void wait(const std::chrono::milliseconds& waitTime);
	virtual bool skipRun();

	virtual void onRun();
	virtual void onStop();

public:
	FixedDelaySchedulerThread(const std::shared_ptr<Runnable>& runnable, const std::chrono::milliseconds& firstDelay, const std::chrono::milliseconds& delay, const std::string& name);
	virtual ~FixedDelaySchedulerThread();

	void setSynchronizationObjectName(const std::string& syncName);

	static std::string periodText(const std::chrono::milliseconds& t);
};

///////////////////////////////////////////////////////////////////////////////

class EveryHourSchedulerThread : public FixedDelaySchedulerThread {
	std::tm lastRun;

	virtual bool skipRun() override;

public:
	EveryHourSchedulerThread(const std::shared_ptr<Runnable>& runnable, const std::string& name);
	virtual ~EveryHourSchedulerThread();
};

///////////////////////////////////////////////////////////////////////////////

class EveryDaySchedulerThread : public FixedDelaySchedulerThread {
	std::tm lastRun;

	virtual bool skipRun() override;

public:
	EveryDaySchedulerThread(const std::shared_ptr<Runnable>& runnable, const std::string& name);
	virtual ~EveryDaySchedulerThread();
};
