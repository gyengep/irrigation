#include "TimerThread.h"

using namespace std;


TimerThread::TimerThread(const string& name) : Thread(name),
	delay(0),
	stopped(false)
{
}

TimerThread::~TimerThread() {
}

void TimerThread::schedule(const std::chrono::milliseconds& delay) {
	unique_lock<mutex> lock(mtx);
	this->delay = delay;
}

void TimerThread::stop() {
	{
		unique_lock<mutex> lock(mtx);
		stopped = true;
		cv.notify_all();
	}

	join();
}

void TimerThread::onExecute() {
	unique_lock<mutex> lock(mtx);

	while (!stopped) {
		cv.wait_for(lock, delay, [this]() { return stopped; });

		if (!stopped) {
			onTimer();
		}
	}
}
