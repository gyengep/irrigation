#include "FixedDelaySchedulerThread.h"
#include "TimeConversion.h"
#include "Exceptions/InterruptedException.h"
#include "Logger/Logger.h"
#include <chrono>

using namespace std;


FixedDelaySchedulerThread::FixedDelaySchedulerThread(const std::shared_ptr<Runnable>& runnable, const std::chrono::milliseconds& firstDelay, const std::chrono::milliseconds& delay, const string& name) :
	Thread(runnable, name),
	firstDelay(firstDelay),
	delay(delay),
	stopped(false)
{
}

FixedDelaySchedulerThread::~FixedDelaySchedulerThread() {
}

void FixedDelaySchedulerThread::setSynchronizationObjectName(const std::string& syncName) {
	synchronizationObject.setName(syncName);
}

void FixedDelaySchedulerThread::wait(const std::chrono::milliseconds& waitTime) {
	auto lock = synchronizationObject.synchronize();
	synchronizationObject.wait_for(waitTime, [this] { return stopped; });
}

bool FixedDelaySchedulerThread::skipRun() {
	return false;
}

void FixedDelaySchedulerThread::onRun() {

	if (LOGGER.isLoggable(LogLevel::DEBUG)) {
		LOGGER.debug("FixedDelayScheduler [%s] is started with delay: %s, first delay: %s", getName().c_str(), periodText(delay).c_str(), periodText(firstDelay).c_str());
	}

	try {
		wait(firstDelay);
		while (true) {

			if (!skipRun()) {
				Thread::onRun();
			} else {
				LOGGER.trace("FixedDelayScheduler [%s] run skipped", getName().c_str());
			}

			wait(delay);
		}
	} catch (const InterruptedException&) {
		LOGGER.debug("FixedDelayScheduler [%s] is interrupted", getName().c_str());
	}
}

void FixedDelaySchedulerThread::onStop() {
	auto lock = synchronizationObject.synchronize();

	stopped = true;
	Thread::onStop();
	synchronizationObject.notify_all();
}

std::string FixedDelaySchedulerThread::periodText(const std::chrono::milliseconds& t) {
	string periodText;

	if (t < chrono::hours(1)) {
		periodText = std::to_string(std::chrono::duration_cast<std::chrono::seconds>(t).count()) + " seconds";
	} else {
		periodText = std::to_string(std::chrono::duration_cast<std::chrono::hours>(t).count()) + " hours";
	}

	return periodText;
}

///////////////////////////////////////////////////////////////////////////////

EveryHourSchedulerThread::EveryHourSchedulerThread(const std::shared_ptr<Runnable>& runnable, const std::string& name) :
	FixedDelaySchedulerThread(runnable, std::chrono::minutes(1), std::chrono::minutes(1), name),
	lastRun(toLocalTime(time(nullptr)))
{
}

EveryHourSchedulerThread::~EveryHourSchedulerThread() {
}

bool EveryHourSchedulerThread::skipRun() {
	const auto current = toLocalTime(time(nullptr));

	bool differentHour = false;

	differentHour |= (current.tm_year != lastRun.tm_year);
	differentHour |= (current.tm_yday != lastRun.tm_yday);
	differentHour |= (current.tm_hour != lastRun.tm_hour);

	if (differentHour) {
		lastRun = current;
	}

	return !differentHour;
}

///////////////////////////////////////////////////////////////////////////////

EveryDaySchedulerThread::EveryDaySchedulerThread(const std::shared_ptr<Runnable>& runnable, const std::string& name) :
	FixedDelaySchedulerThread(runnable, std::chrono::minutes(1), std::chrono::minutes(1), name),
	lastRun(toLocalTime(time(nullptr)))
{
}

EveryDaySchedulerThread::~EveryDaySchedulerThread() {
}

bool EveryDaySchedulerThread::skipRun() {
	const auto current = toLocalTime(time(nullptr));

	bool differentDay = false;

	differentDay |= (current.tm_year != lastRun.tm_year);
	differentDay |= (current.tm_yday != lastRun.tm_yday);

	if (differentDay) {
		lastRun = current;
	}

	return !differentDay;
}
