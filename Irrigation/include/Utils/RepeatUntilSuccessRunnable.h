#pragma once
#include <chrono>
#include <memory>
#include <vector>
#include "Runnable.h"
#include "SynchronizationObject.h"


class RepeatUntilSuccessRunnable : public Runnable {
	const std::shared_ptr<Runnable> runnable;
	const std::vector<std::chrono::milliseconds> delayOnFailed;
	const std::string name;

	SynchronizationObject synchronizationObject;

	bool interrupted;

public:
	RepeatUntilSuccessRunnable(
			const std::shared_ptr<Runnable>& runnable,
			const std::vector<std::chrono::milliseconds>& delayOnFailed,
			const std::string& name
		);
	virtual ~RepeatUntilSuccessRunnable();

	virtual void run() override;
	virtual void interrupt() override;

	void setSynchronizationObjectName(const std::string& syncName);
};
