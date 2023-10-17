#include "Impl/ZoneHandlerImpl.h"
#include "Exceptions/Exceptions.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

ZoneHandlerImpl::ZoneHandlerImpl(const std::vector<std::shared_ptr<Valve>>& valves) :
	masterValve(valves.back()),
	zoneValves(valves.begin(), valves.end() - 1),
	activeZoneId(invalidZoneId)
{
	if (getZoneCount() + 1 != valves.size()) {
		throw invalid_argument("ZoneHandlerImpl::ZoneHandlerImpl() valves.size() must be " + std::to_string(getZoneCount() + 1));
	}

	for (size_t i = 0; i < valves.size(); i++) {
		if (nullptr == valves[i]) {
			throw invalid_argument("ZoneHandlerImpl::ZoneHandlerImpl() valves[" + std::to_string(i) + "] pointer cannot be NULL");
		}
	}
}

ZoneHandlerImpl::~ZoneHandlerImpl() {
	deactivate();
}

size_t ZoneHandlerImpl::getActiveId() const {
	return activeZoneId;
}

void ZoneHandlerImpl::activate(size_t zoneId) {
	if (zoneValves.size() <= zoneId) {
		throw IndexOutOfBoundsException(
				"Zone index shall be less than " + to_string(zoneValves.size()) +
				", while actual value is " + to_string(zoneId));
	}

	if (invalidZoneId != activeZoneId) {
		zoneValves[activeZoneId]->deactivate();
	} else {
		masterValve->activate();
	}

	activeZoneId = zoneId;
	zoneValves[activeZoneId]->activate();
}

void ZoneHandlerImpl::deactivate() {
	if (invalidZoneId != activeZoneId) {
		zoneValves[activeZoneId]->deactivate();
		masterValve->deactivate();
		activeZoneId = invalidZoneId;
	}
}
