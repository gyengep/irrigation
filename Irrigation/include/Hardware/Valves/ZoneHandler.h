#pragma once
#include <cstdlib>


class ZoneHandler {
public:
	static const size_t invalidZoneId;
	static size_t getZoneCount();

	virtual ~ZoneHandler() = default;
	virtual void activate(size_t zoneId) = 0;
	virtual void deactivate() = 0;
	virtual size_t getActiveId() const = 0;
};
