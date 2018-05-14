#include "ZoneHandler.h"
#include <vector>
#include "Hardware/Valves.h"


#if (ZONE_COUNT + 1) != (VALVE_COUNT)
#error (ZONE_COUNT + 1) != (VALVE_COUNT)
#endif


ZoneHandler::ZoneHandler() :
	activeZoneId(invalidZoneId)
{
}

ZoneHandler::~ZoneHandler() {
	deactivate();
}

void ZoneHandler::activateOrDeactivateValves(size_t zoneId, bool activate) {
	std::vector<size_t> valves { zoneValves[zoneId], masterValve };
	Valves::getInstance().activate(valves, activate);
}

size_t ZoneHandler::getActiveId() const {
	std::lock_guard<std::mutex> lock(mutex);
	return activeZoneId;
}

void ZoneHandler::activate(size_t zoneId) {
	if (zoneValves.size() <= zoneId) {
		throw std::out_of_range("Invalid zoneID: " + std::to_string(zoneId));
	}

	std::lock_guard<std::mutex> lock(mutex);

	if (invalidZoneId != activeZoneId) {
		activateOrDeactivateValves(activeZoneId, false);
	}

	activeZoneId = zoneId;
	activateOrDeactivateValves(activeZoneId, true);
}

void ZoneHandler::deactivate() {
	std::lock_guard<std::mutex> lock(mutex);

	if (invalidZoneId != activeZoneId) {
		activateOrDeactivateValves(activeZoneId, false);
		activeZoneId = invalidZoneId;
	}
}
