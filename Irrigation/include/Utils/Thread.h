#pragma once
#include <memory>
#include <string>
#include <thread>
#include "Runnable.h"


class Thread {
	const std::shared_ptr<Runnable> runnable;
	const std::string name;

	Runnable* runnablePtr;
	std::thread workerThread;

	void workerFunction();

public:
	Thread(const std::shared_ptr<Runnable>& runnable, const std::string& name);
	Thread(Runnable& runnable, const std::string& name);
	virtual ~Thread();

	void start();
	void stop();
};
