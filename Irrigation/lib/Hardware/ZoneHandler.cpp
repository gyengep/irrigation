#include "ZoneHandler.h"
#include "Valves.h"
#include <limits>
#include <vector>
#include "Exceptions/Exceptions.h"

using namespace std;


#if (ZONE_COUNT + 1) != (VALVE_COUNT)
#error (ZONE_COUNT + 1) != (VALVE_COUNT)
#endif


mutex ZoneHandler::createMutex;
shared_ptr<ZoneHandler> ZoneHandler::instance;

const shared_ptr<ZoneHandler> ZoneHandler::getInstancePtr() {
	if (nullptr == instance) {
		lock_guard<std::mutex> lock(createMutex);

		if (nullptr == instance) {
			instance.reset(new ZoneHandler(Valves::getInstancePtr()));
		}
	}

	return instance;
}

///////////////////////////////////////////////////////////////////////////////

const size_t ZoneHandler::invalidZoneId = numeric_limits<size_t>::max();


ZoneHandler::ZoneHandler(std::shared_ptr<Valves> valves) :
	valves(valves),
	activeZoneId(invalidZoneId)
{
	if (valves == nullptr) {
		throw invalid_argument("ZoneHandler::ZoneHandler() valves pointer cannot be NULL");
	}
}

ZoneHandler::~ZoneHandler() {
	deactivate();
}

void ZoneHandler::activateOrDeactivateValves(size_t zoneId, bool activate) {
	const size_t size = 2;
	const size_t valveIds[size] { zoneValves[zoneId], masterValve };
	valves->activate(valveIds, size, activate);
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
