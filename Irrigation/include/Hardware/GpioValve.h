#pragma once
#include "Valve.h"


class GpioValve: public Valve {
	const int pin;

public:
	GpioValve(int pin);
	virtual ~GpioValve();

	virtual void activate();
	virtual void deactivate();
};

///////////////////////////////////////////////////////////////////////////////

class GpioValveFactory : public ValveFactory {
public:
	virtual std::unique_ptr<Valve> createValve(size_t id);
};
