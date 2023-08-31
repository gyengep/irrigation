#include "WateringControllerImpl.h"
#include "Logger/Logger.h"
#include <stdexcept>


WateringControllerImpl::WateringControllerImpl(const std::shared_ptr<ZoneHandler>& zoneHandler) :
	zoneHandler(zoneHandler),
	stopped(false),
	active(false)
{
}

WateringControllerImpl::~WateringControllerImpl() {
	std::unique_lock<std::mutex> lock(mtx);

	if (workerThread.joinable()) {
		stopped = true;
		condition.notify_all();
		condition.wait(lock, [this]{ return !active; });
		workerThread.join();
	}
}

void WateringControllerImpl::start(const DurationList& durations) {
	std::list<std::chrono::milliseconds> result;

	for (auto it = durations.begin(); durations.end() != it; ++it) {
		result.emplace_back(*it);
	}

	startMs(result);
}

void WateringControllerImpl::startMs(const std::list<std::chrono::milliseconds>& durations) {

	if (durations.size() != ZoneHandler::getZoneCount()) {
		throw std::runtime_error("Number of durations has to be " + std::to_string(ZoneHandler::getZoneCount()));
	}

	std::unique_lock<std::mutex> lock(mtx);

	if (workerThread.joinable()) {
		stopped = true;
		condition.notify_all();
		condition.wait(lock, [this]{ return !active; });
		workerThread.join();
	}

	LOGGER.info("Irrigation is started: \n\t%s",
		DurationList(durations).toString().c_str()
	);

	stopped = false;
	active = true;
	workerThread = std::thread(&WateringControllerImpl::workerFunc, this, durations);
}

void WateringControllerImpl::stop() {
	std::unique_lock<std::mutex> lock(mtx);

	if (workerThread.joinable()) {
		stopped = true;
		condition.notify_all();
		condition.wait(lock, [this]{ return !active; });
		workerThread.join();
	}
}

void WateringControllerImpl::workerFunc(const std::list<std::chrono::milliseconds> durations) {
	std::unique_lock<std::mutex> lock(mtx);

	size_t i = 0;
	for (auto it = durations.begin(); durations.end() != it; ++it, ++i) {

		if (*it > std::chrono::milliseconds(0)) {
			zoneHandler->activate(i);
			LOGGER.debug("Zone[%u] is activated", i);

			condition.wait_for(lock, *it, [this]{ return stopped; });
			if (stopped) {
				break;
			}
		}
	}

	zoneHandler->deactivate();
	active = false;
	condition.notify_all();
	LOGGER.info("Irrigation is finished");
}

bool WateringControllerImpl::isWateringActive() const {
	std::unique_lock<std::mutex> lock(mtx);
	return active;
}
