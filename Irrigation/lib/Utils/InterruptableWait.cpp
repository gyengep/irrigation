#include "InterruptableWait.h"

using namespace std;


InterruptableWait::InterruptableWait() :
	finished(false)
{
}

void InterruptableWait::wait_for(const std::chrono::milliseconds& ms) {
	unique_lock<mutex> lock(mtx);

	cv.wait_for(lock, ms, [this] {
		return finished;
	});
}

void InterruptableWait::finish() {
	finished = true;
	cv.notify_all();
}
