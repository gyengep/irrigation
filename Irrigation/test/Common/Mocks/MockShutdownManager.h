#pragma once
#include <gmock/gmock.h>
#include "Utils/ShutdownManager.h"


class MockShutdownManager : public ShutdownManager {
public:

	MOCK_METHOD0(powerOff, void());
	MOCK_METHOD0(reboot, void());
};
