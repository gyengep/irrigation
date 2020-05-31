#pragma once
#include <string>
#include <thread>


class Thread {
	const std::string name;
	std::thread workerThread;

	void workerFunction();

	virtual void onExecute() = 0;

public:
	Thread(const std::string& name = "");
	virtual ~Thread();

	void start();
	void join();

	const std::string& getName() const;
};
