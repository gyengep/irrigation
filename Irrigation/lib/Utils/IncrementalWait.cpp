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

void IncrementalWait::incrementWaitTime() {
	if (waitTimeIdx + 1 < waitTimes.size()) {
		waitTimeIdx += 1;
	}
}

void IncrementalWait::wait() {
	interruptableWait.wait_for(waitTimes[waitTimeIdx]);
}

void IncrementalWait::interrupt() {
	interruptableWait.interrupt();
}

