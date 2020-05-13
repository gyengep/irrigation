#pragma once
#include "Valve.h"


class GpioValve: public Valve {
	const int pin;

public:
	GpioValve(int pin);
	virtual ~GpioValve();

	virtual void activate() override;
	virtual void deactivate() override;
};

///////////////////////////////////////////////////////////////////////////////

class GpioValveFactory : public ValveFactory {
public:
	virtual std::shared_ptr<Valve> createValve(size_t id) override;
};
