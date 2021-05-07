#pragma once
#include <memory>
#include <string>
#include <thread>
#include "Runnable.h"


class Thread {
	const std::shared_ptr<Runnable> runnable;
	const std::string name;

	std::thread workerThread;

	void workerFunction();

public:

	enum class Priority {
		NORMAL,
		HIGH
	};

	Thread(const std::shared_ptr<Runnable>& runnable, const std::string& name);
	virtual ~Thread();

	void start();
	void start(Priority priority);
	void stop();
};
