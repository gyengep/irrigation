#include "WateringController.h"
#include <stdexcept>
#include <thread>
#include "Logger/Logger.h"
#include "Utils/ToString.h"

using namespace std;


WateringController::WateringProperties::WateringProperties() :
	zoneStartTime(0)
{
}

WateringController::WateringController(shared_ptr<ZoneHandler> zoneHandler) :
	zoneHandler(zoneHandler)
{
	if (zoneHandler == nullptr) {
		throw invalid_argument("ZoneHandler::ZoneHandler() valves pointer cannot be NULL");
	}
}

WateringController::~WateringController() {
}

void WateringController::on1SecTimer(const time_t& rawTime) {
	if (wateringProperties.get() != nullptr) {
		const IdType id = zoneHandler->getActiveId();
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
	return zoneHandler->getActiveId();
}

void WateringController::startNextRequiredZone(const time_t& rawTime) {
	size_t idx = zoneHandler->getActiveId();

	if (ZoneHandler::invalidZoneId == idx) {
		idx = 0;
	} else {
		idx++;
	}

	while (idx < wateringProperties->runTimes.size()) {
		if (wateringProperties->runTimes[idx].getValue() > 0) {
			wateringProperties->zoneStartTime = rawTime;
			zoneHandler->activate(idx);
			LOGGER.debug("Zone %u activated", idx);
			return;
		}

		idx++;
	}

	zoneHandler->deactivate();
	wateringProperties.reset();
	LOGGER.info("Irrigation finished");
}
