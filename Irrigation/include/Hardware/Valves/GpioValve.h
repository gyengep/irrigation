#pragma once
#include <memory>
#include <vector>
#include "Valve.h"


class GpioValve: public Valve {
	static const std::vector<std::shared_ptr<GpioValve>> valves;

	const int pin;

	void setPin(int mode);

public:
	GpioValve(int pin);
	virtual ~GpioValve();

	virtual void activate() override;
	virtual void deactivate() override;

	static void init();
	static std::vector<std::shared_ptr<Valve>> getValves();
};
