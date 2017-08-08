#pragma once

#include <array>
#include <memory>

#include "Utils/Factory.h"


class ValvesFactory;


class Valves {
	friend ValvesFactory;

	static const int PIN_COUNT = 7;

	static std::mutex createMutex;
	static std::unique_ptr<Factory<Valves>> factory;
	static std::unique_ptr<Valves> instance;
	static const std::array<int, PIN_COUNT> pins;

	mutable std::mutex mutex;

	void activate_notSafe(size_t valveID, bool active);

protected:

	Valves();

public:
	virtual ~Valves();

	virtual void activate(size_t valveID, bool active);
	virtual void activate(size_t valveID1, size_t valveID2, bool active);
	
	static void init();
	static Valves& getInstance();
	static void setFactory(Factory<Valves>* valvesFactory);
};

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

class ValvesFactory : public Factory<Valves> {
public:
	virtual Valves* create() { return new Valves(); }
};

