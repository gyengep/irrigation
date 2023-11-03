#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "Logic/Impl/RunTimeContainerImpl.h"
#include "Mocks/MockRunTime.h"


class RunTimeContainerImplTest : public ::testing::Test {
protected:

	std::shared_ptr<RunTimeContainerImpl> runTimeContainer;
	std::shared_ptr<MockRunTimeFactory> mockRunTimeFactory;

    virtual void SetUp();
    virtual void TearDown();
};

///////////////////////////////////////////////////////////////////////////////

class RunTimeContainerImplDtoTest : public RunTimeContainerImplTest {
protected:

	static const RunTimeDtoList sample;
};
