#include "WateringController.h"
#include "Hardware/Valves/ZoneHandler.h"
#include "Logger/Logger.h"
#include "Utils/ToString.h"
#include <stdexcept>

using namespace std;


WateringController::WateringController() :
	WateringController(ZoneHandler::getInstancePtr())
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

void WateringController::start(const RunTimeContainer& runTimes, unsigned adjustmentPercent) {
	unique_lock<mutex> lock(mtx);

	if (workerThread.joinable()) {
		stopped = true;
		condition.notify_all();
		condition.wait(lock, [this]{ return !active; });
		workerThread.join();
	}

	const auto adjustedRunTimes = adjustRunTimes(runTimes, adjustmentPercent);

	if (LOGGER.isLoggable(LogLevel::DEBUG)) {
		LOGGER.debug("Irrigation started with parameters: \n"
			"\tadjustment: %u%%, runTimes: %s\n"
			"\tadjusted runTimes: %s",
			adjustmentPercent, to_string(runTimes).c_str(),
				to_string(
					adjustedRunTimes.begin(),
					adjustedRunTimes.end()).c_str()
				);
	} else {
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

void WateringController::workerFunc(const vector<RunTime> runTimes) {
	unique_lock<mutex> lock(mtx);

	for (size_t i = 0; i < zoneHandler->getZoneCount(); ++i) {

		if (runTimes[i].getMilliSeconds() > 0) {
			zoneHandler->activate(i);
			LOGGER.debug("Zone[%u] activated", i);

			condition.wait_for(lock, chrono::milliseconds(runTimes[i].getMilliSeconds()), [this]{ return stopped; });
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

vector<RunTime> WateringController::adjustRunTimes(const RunTimeContainer& runTimes, unsigned adjustmentPercent) {
	vector<RunTime> adjustedRunTimes(runTimes.size());

	for (size_t i = 0; i < runTimes.size(); ++i) {
		unsigned adjustedMilliSeconds = runTimes.at(i)->getMilliSeconds() * adjustmentPercent / 100;
		adjustedRunTimes[i].setMilliSeconds(adjustedMilliSeconds);
	}

	return adjustedRunTimes;
}
