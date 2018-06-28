#pragma once
#include <array>
#include <memory>
#include <mutex>
#include <vector>
#include "ValveConfig.h"

class GpioHandler;


class Valves {
	static std::mutex createMutex;
	static std::shared_ptr<Valves> instance;

	const std::array<int, VALVE_COUNT> pins {
		VALVE0_PIN,
		VALVE1_PIN,
		VALVE2_PIN,
		VALVE3_PIN,
		VALVE4_PIN,
		VALVE5_PIN,
		VALVE6_PIN
	};

	mutable std::mutex mutex;

protected:

	std::shared_ptr<GpioHandler> gpioHandler;

	void checkId(size_t valveID);
	void activateWithoutLock(size_t valveID, bool active);

public:
	Valves(std::shared_ptr<GpioHandler> gpioHandler);
	~Valves();

	void activate(size_t valveID, bool active);
	void activate(const size_t* valveIDs, size_t size, bool active);
	
	size_t getCount() const { return pins.size(); }

	static const std::shared_ptr<Valves> getInstancePtr();
};

