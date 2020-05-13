#pragma once
#include <memory>


class ZoneHandler {
public:
	static const size_t invalidZoneId;

	virtual ~ZoneHandler() = default;

	virtual void activate(size_t zoneId) = 0;
	virtual void deactivate() = 0;
	virtual size_t getActiveId() const = 0;

	static size_t getZoneCount();
	static const std::shared_ptr<ZoneHandler> getInstancePtr();
};
