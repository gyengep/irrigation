#include "InterruptableWait.h"


#include <thread>




using namespace std;


InterruptableWait::InterruptableWait() :
	finished(false)
{
}

void InterruptableWait::wait_for(const std::chrono::milliseconds& ms) {
	unique_lock<mutex> lock(mtx);
	//std::this_thread::sleep_for(ms);

	cv.wait_for(lock, ms, [this] {
		return finished;
	});
}

void InterruptableWait::finish() {
	finished = true;
	cv.notify_all();
}
