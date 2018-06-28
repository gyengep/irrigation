#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "MockGpioHandler.h"
#include "Hardware/ZoneHandler.h"



class ZoneHandlerTest : public ::testing::Test {
protected:

	std::shared_ptr<MockGpioHandler> mockGpioHandler;
	std::unique_ptr<ZoneHandler> zoneHandler;

    virtual void SetUp();
    virtual void TearDown();
};
