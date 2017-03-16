#include "common.h"
#include "Zones.h"

#include "Hardware/Valves.h"


Zones::Zones(size_t masterValve, const std::array<size_t, ZONE_COUNT>& zoneValvesPPP) :
	masterValve(masterValve),
	zoneValves(zoneValvesPPP)
{
}

Zones::~Zones() {
	std::lock_guard<std::mutex> lock(mutex);

	for (size_t i = 0; i < zoneValves.size(); ++i) {
		Valves::getInstance().activate(zoneValves[i], masterValve, false);
	}
}

void Zones::activate(size_t zoneID, bool active) {
	std::lock_guard<std::mutex> lock(mutex);

	if (zoneValves.size() <= zoneID) {
		throw std::out_of_range("Invalid zoneID: " + std::to_string(zoneID));
	}

	Valves::getInstance().activate(zoneValves[zoneID], masterValve, active);
}
