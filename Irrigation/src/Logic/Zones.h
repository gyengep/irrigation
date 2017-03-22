#pragma once

#include <mutex>


class Zones {
	mutable std::mutex mutex;
	size_t masterValve;
	const std::array<size_t, ZONE_COUNT> zoneValves;

public:
	Zones(size_t masterValve, const std::array<size_t, ZONE_COUNT>& zoneValves);
	~Zones();

	void activate(size_t zoneID, bool active);
};
