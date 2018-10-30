#pragma once
#include <limits>
#include <memory>
#include <vector>
#include "Valve.h"


class ZoneHandler {
public:
	class Builder;

	static const size_t invalidZoneId = std::numeric_limits<size_t>::max();

private:
	std::unique_ptr<Valve> masterValve;
	std::vector<std::unique_ptr<Valve>> zoneValves;
	size_t activeZoneId;

	ZoneHandler(std::unique_ptr<ValveFactory>&& valveFactory);

public:
	~ZoneHandler();

	void activate(size_t zoneId);
	void deactivate();

	size_t getActiveId() const;

	static size_t getZoneCount();
	static const std::shared_ptr<ZoneHandler> getInstancePtr();
};

///////////////////////////////////////////////////////////////////////////////

class ZoneHandler::Builder {
	std::unique_ptr<ValveFactory> valveFactory;

public:
	Builder& setValveFactory(std::unique_ptr<ValveFactory>&& valveFactory);

	Builder& setFakeValveFactory();

	std::shared_ptr<ZoneHandler> build();
};
