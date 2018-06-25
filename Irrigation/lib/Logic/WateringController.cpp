#include "WateringController.h"
#include <stdexcept>
#include <thread>
#include "Logger/Logger.h"
#include "Utils/ToString.h"

using namespace std;


WateringController::WateringProperties::WateringProperties() :
	zones(new ZoneHandler()),
	zoneStartTime(0)
{
}

WateringController::WateringController() {
}

WateringController::~WateringController() {
}

void WateringController::on1SecTimer(const time_t& rawTime) {
	if (wateringProperties.get() != nullptr) {
		const IdType id = wateringProperties->zones->getActiveId();
		const unsigned zoneRunTime = wateringProperties->runTimes[id].getValue();
		if (difftime(rawTime, wateringProperties->zoneStartTime) >= zoneRunTime) {
			startNextRequiredZone(rawTime);
		}
	}
}

void WateringController::start(const time_t& rawTime, const RunTimeContainer& runTimes, float adjustment) {
	wateringProperties.reset(new WateringProperties());

	LOGGER.info("Irrigation starting");

	for (size_t i = 0; i < runTimes.size(); ++i) {
		wateringProperties->runTimes[i].setValue(runTimes.at(i)->getValue() * adjustment);
	}

	LOGGER.debug("Irrigation started with the following parameters:");
	LOGGER.debug("adjustment: %f", adjustment);
	LOGGER.debug("runTimes: %s", to_string(runTimes).c_str());
	LOGGER.debug("adjusted runTimes: %s",
			to_string(
				wateringProperties->runTimes.begin(),
				wateringProperties->runTimes.end()).c_str()
			);

	startNextRequiredZone(rawTime);
}

void WateringController::stop() {
	LOGGER.info("Irrigation stopped");
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
		if (wateringProperties->runTimes[idx].getValue() > 0) {
			wateringProperties->zoneStartTime = rawTime;
			wateringProperties->zones->activate(idx);
			LOGGER.debug("Zone %u activated", idx);
			return;
		}

		idx++;
	}

	LOGGER.info("Irrigation finished");
	wateringProperties.reset();
}
