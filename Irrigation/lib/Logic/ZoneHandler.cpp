#include "ZoneHandler.h"
#include <limits>
#include <vector>
#include "Exceptions/Exceptions.h"
#include "Hardware/Valves.h"

using namespace std;


#if (ZONE_COUNT + 1) != (VALVE_COUNT)
#error (ZONE_COUNT + 1) != (VALVE_COUNT)
#endif


const size_t ZoneHandler::invalidZoneId = numeric_limits<size_t>::max();


ZoneHandler::ZoneHandler() :
	activeZoneId(invalidZoneId)
{
}

ZoneHandler::~ZoneHandler() {
	deactivate();
}

void ZoneHandler::activateOrDeactivateValves(size_t zoneId, bool activate) {
	const size_t size = 2;
	size_t valves[size] { zoneValves[zoneId], masterValve };
	Valves::getInstance().activate(valves, size, activate);
}

size_t ZoneHandler::getActiveId() const {
	return activeZoneId;
}

void ZoneHandler::activate(size_t zoneId) {
	if (zoneValves.size() <= zoneId) {
		throw IndexOutOfBoundsException(
				"Zone index shall be less than " + to_string(zoneValves.size()) +
				", while actual value is " + to_string(zoneId));
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
