#include "RepeatUntilSuccessRunnable.h"
#include "TimeContainer.h"
#include "Logger/Logger.h"
#include "Utils/DateTime.h"
#include <sstream>


RepeatUntilSuccessRunnable::RepeatUntilSuccessRunnable(
		const std::shared_ptr<Runnable>& runnable,
		const std::vector<std::chrono::milliseconds>& delayOnFailed,
		const std::string& name
	)
	:
	runnable(runnable),
	delayOnFailed(delayOnFailed),
	name(name),
	synchronizationObject(),
	interrupted(false)
{
	if (nullptr == runnable) {
		throw std::logic_error("RepeatUntilSuccessRunnable::RepeatUntilSuccessRunnable() runnable can not be null");
	}

	if (delayOnFailed.empty()) {
		throw std::invalid_argument("RepeatUntilSuccessRunnable::RepeatUntilSuccessRunnable() delayOnFailed is empty");
	}
}

RepeatUntilSuccessRunnable::~RepeatUntilSuccessRunnable() {
}

void RepeatUntilSuccessRunnable::setSynchronizationObjectName(const std::string& syncName) {
	synchronizationObject.setName(syncName);
}

void RepeatUntilSuccessRunnable::run() {
	const TimeContainer timeContainer(delayOnFailed);
	unsigned errorCount = 0;

	while(true) {
		try {
			runnable->run();
			break;
		} catch (const InterruptedException&) {
			throw;
		} catch (const std::exception& e) {
			const auto waitTime = timeContainer.getNext();

			errorCount++;

			if (LOGGER.isLoggable(LogLevel::WARNING)) {
				const LocalDateTime nextTryDateTime = LocalDateTime::now() + std::chrono::duration_cast<std::chrono::seconds>(waitTime);

				std::ostringstream oss;
				oss << name << " failed on try " << errorCount << ", next try at " << nextTryDateTime.toString("%T");
				LOGGER.warning(oss.str().c_str(), e);
			}

			auto lock = synchronizationObject.synchronize();
			synchronizationObject.wait_for(waitTime, [this]{ return interrupted; });
		}
	}

	if (errorCount > 0) {
		LOGGER.info("%s resumed", name.c_str());
	}
}

void RepeatUntilSuccessRunnable::interrupt() {
	auto lock = synchronizationObject.synchronize();

	interrupted = true;
	runnable->interrupt();
	synchronizationObject.notify_all();
}
