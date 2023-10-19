#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "Logic/Impl/StartTimeContainerImpl.h"
#include "Mocks/MockStartTime.h"


class StartTimeContainerImplTest : public ::testing::Test {
protected:

	std::shared_ptr<StartTimeContainerImpl> startTimeContainer;
	std::shared_ptr<MockStartTimeFactory> mockStartTimeFactory;

    virtual void SetUp();
    virtual void TearDown();
};

///////////////////////////////////////////////////////////////////////////////

class StartTimeContainerImplFromDtoTest : public StartTimeContainerImplTest {
protected:

    void checkUpdateFromStartTimeDtoList();
};
