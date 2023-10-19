#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "Logic/Impl/ProgramContainerImpl.h"
#include "Mocks/MockProgram.h"


class ProgramContainerImplTest : public ::testing::Test {
protected:

	std::shared_ptr<ProgramContainerImpl> programContainer;
	std::shared_ptr<MockProgramFactory> mockProgramFactory;

    virtual void SetUp();
    virtual void TearDown();
};

///////////////////////////////////////////////////////////////////////////////

class ProgramContainerImplFromDtoTest : public ProgramContainerImplTest {
protected:

    void checkUpdateFromProgramDtoList();
};
