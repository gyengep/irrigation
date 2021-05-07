#include "FixedRateSchedulerRunnable.h"
#include "TimeConversion.h"
#include "Exceptions/InterruptedException.h"
#include "Logger/Logger.h"
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std;


FixedRateSchedulerRunnable::FixedRateSchedulerRunnable(const std::shared_ptr<Runnable>& runnable, const std::chrono::milliseconds& period, const std::string& name) :
	runnable(runnable),
	period(period),
	name(name),
	interrupted(false)
{
}

FixedRateSchedulerRunnable::~FixedRateSchedulerRunnable() {
}

void FixedRateSchedulerRunnable::setSynchronizationObjectName(const std::string& syncName) {
	synchronizationObject.setName(syncName);
}

void FixedRateSchedulerRunnable::run() {

	if (LOGGER.isLoggable(LogLevel::DEBUG)) {
		LOGGER.debug("FixedRateSchedulerRunnable [%s] is started with period: %s", name.c_str(), periodText(period).c_str());
	}

	nextScheduleTime = std::chrono::system_clock::now() + period;

	try {
		while (true) {

			{
				auto waitTime = std::chrono::duration_cast<std::chrono::milliseconds>(nextScheduleTime - std::chrono::system_clock::now());

				if (!checkWaitTime(waitTime)) {
					waitTime = period;
					nextScheduleTime = std::chrono::system_clock::now() + period;
				}

				auto lock = synchronizationObject.synchronize();
				synchronizationObject.wait_for(waitTime, [this] { return interrupted; });
			}

			runnable->run();
			nextScheduleTime += period;
		}
	} catch (const InterruptedException&) {
		LOGGER.debug("FixedRateSchedulerRunnable [%s] is interrupted", name.c_str());
	}
}

bool FixedRateSchedulerRunnable::checkWaitTime(const std::chrono::milliseconds& waitTime) const {

	if (waitTime < std::chrono::milliseconds(0) || period < waitTime) {

		if (LOGGER.isLoggable(LogLevel::WARNING)) {
			TimeConverter timeConverter(abs(waitTime));
			std::ostringstream oss;

			oss << "Update period failure in FixedRateScheduler [" << name << "]! ";

			if (waitTime > std::chrono::milliseconds(0)) {
				oss << "-";
			}

			oss << timeConverter.getDays() << " days ";
			oss << std::setw(2) << std::setfill('0') << timeConverter.getHours() << ":";
			oss << std::setw(2) << std::setfill('0') << timeConverter.getMinutes() << ":";
			oss << std::setw(2) << std::setfill('0') << timeConverter.getSeconds() << ".";
			oss << std::setw(3) << std::setfill('0') << timeConverter.getMillis();

			LOGGER.warning(oss.str().c_str());
		}

		return false;
	}

	return true;
}

void FixedRateSchedulerRunnable::interrupt() {
	auto lock = synchronizationObject.synchronize();

	interrupted = true;
	runnable->interrupt();
	synchronizationObject.notify_all();
}

std::string FixedRateSchedulerRunnable::periodText(const std::chrono::milliseconds& t) {
	string periodText;

	if (t < chrono::hours(1)) {
		periodText = std::to_string(std::chrono::duration_cast<std::chrono::seconds>(t).count()) + " seconds";
	} else {
		periodText = std::to_string(std::chrono::duration_cast<std::chrono::hours>(t).count()) + " hours";
	}

	return periodText;
}
