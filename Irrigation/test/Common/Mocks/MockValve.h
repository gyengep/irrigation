#pragma once
#include <gmock/gmock.h>
#include "Hardware/Valves/Valve.h"


class MockValve : public Valve {
public:
	MOCK_METHOD0(activate, void());
	MOCK_METHOD0(deactivate, void());
};
