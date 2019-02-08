#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "Hardware/Valves/Valve.h"
#include "Hardware/Valves/ZoneHandler.h"



class MockValve : public Valve {
public:
	MOCK_METHOD0(activate, void());
	MOCK_METHOD0(deactivate, void());
};

///////////////////////////////////////////////////////////////////////////////

template<class VALVE>
class MockValveFactory : public ValveFactory {
public:
	VALVE* mockMasterValve;
	std::vector<VALVE*> mockZoneValves;

	MockValveFactory();

	virtual std::unique_ptr<Valve> createValve(size_t id) override;
};

template<class VALVE>
MockValveFactory<VALVE>::MockValveFactory() {
	mockZoneValves.resize(ZoneHandler::getZoneCount());

	for (size_t i = 0; i < mockZoneValves.size(); i++) {
		mockZoneValves[i] = new VALVE();
	}

	mockMasterValve = new VALVE();
}

template<class VALVE>
std::unique_ptr<Valve> MockValveFactory<VALVE>::createValve(size_t id) {
	if (id < mockZoneValves.size()) {
		return std::unique_ptr<Valve>(mockZoneValves[id]);
	} else if (id == mockZoneValves.size()) {
		return std::unique_ptr<Valve>(mockMasterValve);
	} else {
		throw std::logic_error("MockValveFactory::createValve() id > mockZoneValves.size()");
	}
}

