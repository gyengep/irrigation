#pragma once
#include <gmock/gmock.h>
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
	std::shared_ptr<VALVE> mockMasterValve;
	std::vector<std::shared_ptr<VALVE>> mockZoneValves;

	MockValveFactory();

	virtual std::shared_ptr<Valve> createValve(size_t id) override;
};

template<class VALVE>
MockValveFactory<VALVE>::MockValveFactory() {
	mockZoneValves.resize(ZoneHandler::getZoneCount());

	for (size_t i = 0; i < mockZoneValves.size(); i++) {
		mockZoneValves[i] = std::shared_ptr<VALVE>(new VALVE());
	}

	mockMasterValve = std::shared_ptr<VALVE>(new VALVE());
}

template<class VALVE>
std::shared_ptr<Valve> MockValveFactory<VALVE>::createValve(size_t id) {
	if (id < mockZoneValves.size()) {
		return mockZoneValves[id];
	} else if (id == mockZoneValves.size()) {
		return mockMasterValve;
	} else {
		throw std::logic_error("MockValveFactory::createValve() id > mockZoneValves.size()");
	}
}

