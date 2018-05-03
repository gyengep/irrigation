#pragma once
#include <array>
#include <memory>
#include <mutex>
#include <vector>
#include "ValveConfig.h"


class Valves {
	static std::mutex createMutex;
	static std::unique_ptr<Valves> instance;

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
	Valves();

	virtual void setPin(int pin, int mode);

public:
	virtual ~Valves();

	virtual void resetAll();
	virtual void activate(size_t valveID, bool active);
	virtual void activate(const std::vector<size_t>& valveIDs, bool active);
	
	size_t getCount() const { return pins.size(); }

	static void init();
	static Valves& getInstance();

	// for testing
	static void setNewInstance(Valves* newInstance);
};

