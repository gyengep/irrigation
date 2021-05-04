#include "FixedRateSchedulerRunnable.h"
//#include "FixedDelayScheduler.h"
#include "TimeConversion.h"
#include "Exceptions/InterruptedException.h"
#include "Logger/Logger.h"
/*

FixedRateSchedulerThread::FixedRateSchedulerThread(const std::shared_ptr<Runnable>& runnable, const std::chrono::milliseconds& period, const std::string& name) :
	Thread(runnable, name),
	period(period),
	stopped(false)
{
}

FixedRateSchedulerThread::~FixedRateSchedulerThread() {
}

void FixedRateSchedulerThread::onRun() {

	if (LOGGER.isLoggable(LogLevel::DEBUG)) {
		LOGGER.debug("FixedRateScheduler [%s] is started with period: %s", getName().c_str(), FixedDelaySchedulerThread::periodText(period).c_str());
	}

	try {
		wait(period);
		while (true) {

			Thread::onRun();
			//wait(calculateNextWait());

		}
	} catch (const InterruptedException&) {
		LOGGER.debug("FixedRateScheduler [%s] is interrupted", getName().c_str());
	}
}

void FixedRateSchedulerThread::onStop() {
	auto lock = synchronizationObject.synchronize();

	stopped = true;
	Thread::onStop();
	synchronizationObject.notify_all();
}
*/
