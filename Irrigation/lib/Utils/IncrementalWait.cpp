#include "IncrementalWait.h"
#include "Exceptions/IllegalArgumentException.h"


IncrementalWait::IncrementalWait(const std::vector<std::chrono::milliseconds>& waitTimes) :
	waitTimes(waitTimes),
	waitTimeIdx(0)
{
	if (waitTimes.empty()) {
		throw IllegalArgumentException("WaitTimes has to be at least one element");
	}
}

void IncrementalWait::resetWaitTime() {
	waitTimeIdx = 0;
}

std::chrono::milliseconds IncrementalWait::getWaitTime() const {
	return waitTimes[waitTimeIdx];
}


void IncrementalWait::wait() {
	interruptableWait.wait_for(waitTimes[waitTimeIdx]);

	if (waitTimeIdx + 1 < waitTimes.size()) {
		waitTimeIdx += 1;
	}
}

void IncrementalWait::interrupt() {
	interruptableWait.interrupt();
}

