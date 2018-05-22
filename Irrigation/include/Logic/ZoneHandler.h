#pragma once
#include <array>
#include <limits>
#include <mutex>
#include "ZoneConfig.h"


class ZoneHandler {
public:
	static const size_t invalidZoneId = std::numeric_limits<size_t>::max();

private:
	mutable std::mutex mutex;

	const size_t masterValve = MASTER_VALVE_ID;
	const std::array<size_t, ZONE_COUNT> zoneValves {
		ZONE0_VALVE_ID,
		ZONE1_VALVE_ID,
		ZONE2_VALVE_ID,
		ZONE3_VALVE_ID,
		ZONE4_VALVE_ID,
		ZONE5_VALVE_ID
	};

	size_t activeZoneId;

	void activateOrDeactivateValves(size_t zoneId, bool activate);

public:
	ZoneHandler();
	~ZoneHandler();

	void activate(size_t zoneId);
	void deactivate();

	size_t getActiveId() const;
	size_t getCount() const { return zoneValves.size(); }
};
