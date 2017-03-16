#pragma once

#include <array>

class Valves {
	static const int PIN_COUNT = 7;

	static std::unique_ptr<Valves> instance;
	static const std::array<int, PIN_COUNT> pins;

	mutable std::mutex mutex;

	Valves();
	void activate_notSafe(size_t valveID, bool active);

public:
	virtual ~Valves();

	void activate(size_t valveID, bool active);
	void activate(size_t valveID1, size_t valveID2, bool active);
	
	static void init();
	static Valves& getInstance();
};
