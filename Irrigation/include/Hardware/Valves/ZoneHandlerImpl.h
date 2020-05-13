#pragma once
#include <memory>
#include <vector>
#include "Valve.h"
#include "ZoneHandler.h"


class ZoneHandlerImpl : public ZoneHandler {
public:
	class Builder;

private:
	std::shared_ptr<Valve> masterValve;
	std::vector<std::shared_ptr<Valve>> zoneValves;
	size_t activeZoneId;

	ZoneHandlerImpl(const std::shared_ptr<ValveFactory>& valveFactory);

public:
	virtual ~ZoneHandlerImpl();

	virtual void activate(size_t zoneId) override;
	virtual void deactivate() override;
	virtual size_t getActiveId() const override;
};

///////////////////////////////////////////////////////////////////////////////

class ZoneHandlerImpl::Builder {
	std::shared_ptr<ValveFactory> valveFactory;

public:
	Builder& setValveFactory(const std::shared_ptr<ValveFactory>& valveFactory);

	std::shared_ptr<ZoneHandlerImpl> build();
};
