#include "WateringController.h"
#include <stdexcept>
#include <thread>

using namespace std;


WateringController::WateringProperties::WateringProperties() :
	zones(new ZoneHandler()),
	zoneStartTime(0)
{
	runTimes.fill(0);
}

WateringController::WateringController() {
}

WateringController::~WateringController() {
}

void WateringController::on1SecTimer(const time_t& rawTime) {
	if (wateringProperties.get() != nullptr) {
		const IdType id = wateringProperties->zones->getActiveId();
		const unsigned zoneRunTime = wateringProperties->runTimes[id];
		if (difftime(rawTime, wateringProperties->zoneStartTime) >= zoneRunTime) {
			startNextRequiredZone(rawTime);
		}
	}
}

void WateringController::start(const time_t& rawTime, const RunTimeContainer& runTimes, float adjustment) {
	wateringProperties.reset(new WateringProperties());

	for (size_t i = 0; i < runTimes.size(); ++i) {
		wateringProperties->runTimes[i] = runTimes.at(i)->getValue() * adjustment;
	}

	startNextRequiredZone(rawTime);
}

void WateringController::stop() {
	wateringProperties.reset();
}

bool WateringController::isWateringActive() const {
	return (wateringProperties.get() != nullptr);
}

size_t WateringController::getActiveZoneId() const {
	if (wateringProperties.get() != nullptr) {
		return wateringProperties->zones->getActiveId();
	} else {
		return ZoneHandler::invalidZoneId;
	}
}

void WateringController::startNextRequiredZone(const time_t& rawTime) {
	size_t idx = wateringProperties->zones->getActiveId();

	if (ZoneHandler::invalidZoneId == idx) {
		idx = 0;
	} else {
		idx++;
	}

	while (idx < wateringProperties->runTimes.size()) {
		if (wateringProperties->runTimes[idx] > 0) {
			wateringProperties->zoneStartTime = rawTime;
			wateringProperties->zones->activate(idx);
			return;
		}

		idx++;
	}

	wateringProperties.reset();
}
