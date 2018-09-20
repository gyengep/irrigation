#pragma once
#include <condition_variable>
#include <mutex>



class Application {

	std::condition_variable condition;
	std::mutex mtx;
	bool isTerminated;

protected:
	virtual void onInitialize();
	virtual void onTerminate();

public:
	Application();
	virtual ~Application();

	void run();
	void exit();
};
