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


class MockValveFactory : public ValveFactory {
public:
	MockValve* mockMasterValve;
	std::vector<MockValve*> mockZoneValves;

	MockValveFactory();

	virtual std::unique_ptr<Valve> createValve(size_t id);
};
