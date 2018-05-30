#include "ZoneHandler.h"
#include <limits>
#include <vector>
#include "Hardware/Valves.h"

using namespace std;


#if (ZONE_COUNT + 1) != (VALVE_COUNT)
#error (ZONE_COUNT + 1) != (VALVE_COUNT)
#endif


const size_t ZoneHandler::invalidZoneId = std::numeric_limits<size_t>::max();


ZoneHandler::ZoneHandler() :
	activeZoneId(invalidZoneId)
{
}

ZoneHandler::~ZoneHandler() {
	deactivate();
}

void ZoneHandler::activateOrDeactivateValves(size_t zoneId, bool activate) {
	vector<size_t> valves { zoneValves[zoneId], masterValve };
	Valves::getInstance().activate(valves, activate);
}

size_t ZoneHandler::getActiveId() const {
	return activeZoneId;
}

void ZoneHandler::activate(size_t zoneId) {
	if (zoneValves.size() <= zoneId) {
		throw out_of_range("Invalid zoneID: " + to_string(zoneId));
	}

	if (invalidZoneId != activeZoneId) {
		activateOrDeactivateValves(activeZoneId, false);
	}

	activeZoneId = zoneId;
	activateOrDeactivateValves(activeZoneId, true);
}

void ZoneHandler::deactivate() {
	if (invalidZoneId != activeZoneId) {
		activateOrDeactivateValves(activeZoneId, false);
		activeZoneId = invalidZoneId;
	}
}
