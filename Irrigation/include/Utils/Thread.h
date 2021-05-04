#pragma once
#include <memory>
#include <string>
#include <thread>
#include "Runnable.h"


class Thread : public Runnable {
	const std::shared_ptr<Runnable> runnable;
	const std::string name;

	std::thread workerThread;

	void workerFunction();

	virtual void run() override;

	Runnable& getRunnable();

protected:

	virtual void onRun();
	virtual void onStop();

public:
	Thread(const std::shared_ptr<Runnable>& runnable, const std::string& name);
	Thread(const std::string& name);
	virtual ~Thread();

	void start();
	void stop();

	const std::string& getName() const;
};
