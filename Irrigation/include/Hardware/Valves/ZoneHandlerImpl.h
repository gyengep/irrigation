#pragma once
#include <memory>
#include <vector>
#include "Valve.h"
#include "ZoneHandler.h"


class ZoneHandlerImpl : public ZoneHandler {
	const std::shared_ptr<Valve> masterValve;
	const std::vector<std::shared_ptr<Valve>> zoneValves;
	size_t activeZoneId;

public:
	ZoneHandlerImpl(const std::vector<std::shared_ptr<Valve>>& valves);
	virtual ~ZoneHandlerImpl();

	virtual void activate(size_t zoneId) override;
	virtual void deactivate() override;
	virtual size_t getActiveId() const override;
};
