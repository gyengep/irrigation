#pragma once
#include <array>
#include <memory>
#include <mutex>
#include "ZoneConfig.h"

class Valves;


class ZoneHandler {
public:
	static const size_t invalidZoneId;

private:

	static std::mutex createMutex;
	static std::shared_ptr<ZoneHandler> instance;

	const size_t masterValve = MASTER_VALVE_ID;
	const std::array<size_t, ZONE_COUNT> zoneValves {
		ZONE0_VALVE_ID,
		ZONE1_VALVE_ID,
		ZONE2_VALVE_ID,
		ZONE3_VALVE_ID,
		ZONE4_VALVE_ID,
		ZONE5_VALVE_ID
	};

	std::shared_ptr<Valves> valves;
	size_t activeZoneId;

	void activateOrDeactivateValves(size_t zoneId, bool activate);

public:
	ZoneHandler(std::shared_ptr<Valves> valves);
	~ZoneHandler();

	void activate(size_t zoneId);
	void deactivate();

	size_t getActiveId() const;
	size_t getCount() const { return zoneValves.size(); }

	static const std::shared_ptr<ZoneHandler> getInstancePtr();
};
