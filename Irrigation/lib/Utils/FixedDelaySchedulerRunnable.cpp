#include "FixedDelaySchedulerRunnable.h"
#include "Exceptions/InterruptedException.h"
#include "Logger/Logger.h"
#include <chrono>

using namespace std;


FixedDelaySchedulerRunnable::FixedDelaySchedulerRunnable(const std::shared_ptr<Runnable>& runnable, const std::chrono::milliseconds& firstDelay, const std::chrono::milliseconds& delay, const std::string& name) :
	runnable(runnable),
	firstDelay(firstDelay),
	delay(delay),
	name(name),
	interrupted(false)
{
}

FixedDelaySchedulerRunnable::~FixedDelaySchedulerRunnable() {
}

void FixedDelaySchedulerRunnable::setSynchronizationObjectName(const std::string& syncName) {
	synchronizationObject.setName(syncName);
}

void FixedDelaySchedulerRunnable::wait(const std::chrono::milliseconds& waitTime) {
	auto lock = synchronizationObject.synchronize();
	synchronizationObject.wait_for(waitTime, [this] { return interrupted; });
}

bool FixedDelaySchedulerRunnable::skipRun() {
	return false;
}

void FixedDelaySchedulerRunnable::run() {

	if (LOGGER.isLoggable(LogLevel::DEBUG)) {
		LOGGER.debug("FixedDelaySchedulerRunnable [%s] is started with delay: %s, first delay: %s", name.c_str(), periodText(delay).c_str(), periodText(firstDelay).c_str());
	}

	try {
		wait(firstDelay);
		while (true) {

			if (!skipRun()) {
				runnable->run();
			} else {
				//LOGGER.trace("FixedDelaySchedulerRunnable [%s] run skipped", name.c_str());
			}

			wait(delay);
		}
	} catch (const InterruptedException&) {
		LOGGER.debug("FixedDelaySchedulerRunnable [%s] is interrupted", name.c_str());
	}
}

void FixedDelaySchedulerRunnable::interrupt() {
	auto lock = synchronizationObject.synchronize();

	interrupted = true;
	runnable->interrupt();
	synchronizationObject.notify_all();
}

std::string FixedDelaySchedulerRunnable::periodText(const std::chrono::milliseconds& t) {
	string periodText;

	if (t < chrono::hours(1)) {
		periodText = std::to_string(std::chrono::duration_cast<std::chrono::seconds>(t).count()) + " seconds";
	} else {
		periodText = std::to_string(std::chrono::duration_cast<std::chrono::hours>(t).count()) + " hours";
	}

	return periodText;
}

///////////////////////////////////////////////////////////////////////////////

EveryHourSchedulerRunnable::EveryHourSchedulerRunnable(const std::shared_ptr<Runnable>& runnable, const std::string& name) :
	FixedDelaySchedulerRunnable(runnable, std::chrono::minutes(1), std::chrono::minutes(1), name),
	lastRun(LocalDateTime::now())
{
}

EveryHourSchedulerRunnable::~EveryHourSchedulerRunnable() {
}

bool EveryHourSchedulerRunnable::skipRun() {
	const LocalDateTime now = DateTime::now();

	bool differentHour = false;

	differentHour |= (now.getYears() != lastRun.getYears());
	differentHour |= (now.getMonths() != lastRun.getMonths());
	differentHour |= (now.getDays() != lastRun.getDays());
	differentHour |= (now.getHours() != lastRun.getHours());

	if (differentHour) {
		lastRun = now;
	}

	return !differentHour;
}

///////////////////////////////////////////////////////////////////////////////

EveryDaySchedulerRunnable::EveryDaySchedulerRunnable(const std::shared_ptr<Runnable>& runnable, const std::string& name) :
	FixedDelaySchedulerRunnable(runnable, std::chrono::minutes(1), std::chrono::minutes(1), name),
	lastRun(LocalDateTime::now())
{
}

EveryDaySchedulerRunnable::~EveryDaySchedulerRunnable() {
}

bool EveryDaySchedulerRunnable::skipRun() {
	const auto now = LocalDateTime::now();

	bool differentDay = false;

	differentDay |= (now.getYears() != lastRun.getYears());
	differentDay |= (now.getMonths() != lastRun.getMonths());
	differentDay |= (now.getDays() != lastRun.getDays());

	if (differentDay) {
		lastRun = now;
	}

	return !differentDay;
}
