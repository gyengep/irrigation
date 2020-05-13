#pragma once
#include <gmock/gmock.h>
#include "Hardware/Valves/ZoneHandler.h"


class MockZoneHandler : public ZoneHandler {
public:
	MOCK_METHOD1(activate, void(size_t zoneId));
	MOCK_METHOD0(deactivate, void());
	MOCK_CONST_METHOD0(getActiveId, size_t());
};
