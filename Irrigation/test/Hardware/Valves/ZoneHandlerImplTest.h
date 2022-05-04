#pragma once
#include <gmock/gmock.h>
#include "Hardware/Valves/ZoneHandlerImpl.h"
#include "Mocks/MockValve.h"


class ZoneHandlerImplTest : public ::testing::Test {
protected:

	std::shared_ptr<ZoneHandlerImpl> zoneHandler;
	std::vector<std::shared_ptr<MockValve>> mockValves;

	virtual void SetUp();
    virtual void TearDown();
};
