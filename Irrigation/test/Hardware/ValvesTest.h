#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "MockGpioHandler.h"
#include "Hardware/GpioHandler.h"
#include "Hardware/Valves.h"



class ValvesTestWithMock : public ::testing::Test {
protected:

	std::shared_ptr<MockGpioHandler> mockGpioHandler;
	std::unique_ptr<Valves> valves;

    virtual void SetUp();
    virtual void TearDown();
};
