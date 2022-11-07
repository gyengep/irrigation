#include "WateringController.h"
#include "Hardware/Valves/GpioValve.h"
#include "Hardware/Valves/ZoneHandlerImpl.h"
#include "Logger/Logger.h"
#include "Utils/ToString.h"
#include <stdexcept>

using namespace std;


WateringController::WateringController() :
	WateringController(std::make_shared<ZoneHandlerImpl>(GpioValve::getValves()))
{
}

WateringController::WateringController(const shared_ptr<ZoneHandler>& zoneHandler) :
	zoneHandler(zoneHandler),
	stopped(false),
	active(false)
{
	if (nullptr == zoneHandler) {
		throw invalid_argument("WateringController::WateringController() zoneHandler pointer cannot be NULL");
	}
}

WateringController::~WateringController() {
	unique_lock<mutex> lock(mtx);

	if (workerThread.joinable()) {
		stopped = true;
		condition.notify_all();
		condition.wait(lock, [this]{ return !active; });
		workerThread.join();
	}
}

void WateringController::start(const std::list<std::chrono::seconds>& runTimes, unsigned adjustmentPercent) {
	std::list<std::chrono::milliseconds> result;

	for (auto it = runTimes.begin(); runTimes.end() != it; ++it) {
		result.emplace_back(*it);
	}

	start(result, adjustmentPercent);
}

void WateringController::start(const std::list<std::chrono::milliseconds>& runTimes, unsigned adjustmentPercent) {

	if (runTimes.size() != ZoneHandler::getZoneCount()) {
		throw std::runtime_error("Number of durations has to be " + std::to_string(ZoneHandler::getZoneCount()));
	}

	unique_lock<mutex> lock(mtx);

	if (workerThread.joinable()) {
		stopped = true;
		condition.notify_all();
		condition.wait(lock, [this]{ return !active; });
		workerThread.join();
	}

	const auto adjustedRunTimes = adjustRunTimes(runTimes, adjustmentPercent);

	/*if (LOGGER.isLoggable(LogLevel::DEBUG)) {
		LOGGER.debug("Irrigation started with parameters: \n"
			"\tadjustment: %u%%, runTimes: %s\n"
			"\tadjusted runTimes: %s",
			adjustmentPercent, to_string(runTimes).c_str(),
				to_string(
					adjustedRunTimes.begin(),
					adjustedRunTimes.end()).c_str()
				);
	} else*/ {
		LOGGER.info("Irrigation started");
	}

	stopped = false;
	active = true;
	workerThread = thread(&WateringController::workerFunc, this, adjustedRunTimes);
}

void WateringController::stop() {
	unique_lock<mutex> lock(mtx);

	if (workerThread.joinable()) {
		stopped = true;
		condition.notify_all();
		condition.wait(lock, [this]{ return !active; });
		workerThread.join();
	}

	LOGGER.info("Irrigation stopped");
}

void WateringController::workerFunc(const std::list<std::chrono::milliseconds> durations) {
	unique_lock<mutex> lock(mtx);

	size_t i = 0;
	for (auto it = durations.begin(); durations.end() != it; ++it, ++i) {

		if (*it > std::chrono::milliseconds(0)) {
			zoneHandler->activate(i);
			LOGGER.debug("Zone[%u] activated", i);

			condition.wait_for(lock, *it, [this]{ return stopped; });
			if (stopped) {
				zoneHandler->deactivate();
				active = false;
				condition.notify_all();
				return;
			}
		}
	}

	zoneHandler->deactivate();
	active = false;
	condition.notify_all();
	LOGGER.info("Irrigation finished");
}

bool WateringController::isWateringActive() const {
	unique_lock<mutex> lock(mtx);
	return active;
}

size_t WateringController::getActiveZoneId() const {
	unique_lock<mutex> lock(mtx);
	return zoneHandler->getActiveId();
}

std::list<std::chrono::milliseconds> WateringController::adjustRunTimes(const std::list<std::chrono::milliseconds>& runTimes, unsigned adjustmentPercent) {
	std::list<std::chrono::milliseconds> adjustedRunTimes;

	for (auto it = runTimes.begin(); runTimes.end() != it; ++it) {
		adjustedRunTimes.emplace_back((*it) * adjustmentPercent / 100);
	}

	return adjustedRunTimes;
}
