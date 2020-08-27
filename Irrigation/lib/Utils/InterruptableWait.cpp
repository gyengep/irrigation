#include "InterruptableWait.h"
#include "Exceptions/InterruptedException.h"

using namespace std;


InterruptableWait::InterruptableWait() :
	interrupted(false)
{
}

void InterruptableWait::wait_for(const std::chrono::milliseconds& ms) {
	unique_lock<mutex> lock(mtx);
	interrupted = false;

	cv.wait_for(lock, ms, [this]() { return interrupted; });

	if (interrupted) {
		throw InterruptedException("InterruptableWait::wait_for() is interrupted");
	}
}

void InterruptableWait::interrupt() {
	unique_lock<mutex> lock(mtx);
	interrupted = true;
	cv.notify_all();
}
