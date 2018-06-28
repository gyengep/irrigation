#pragma once
#include <gmock/gmock.h>
#include "Hardware/GpioHandler.h"



class MockGpioHandler : public GpioHandler {
public:
	MOCK_METHOD2(setPin, void(int, int));
};

