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

class ProgramContainerImplDtoTest : public ProgramContainerImplTest {
protected:
	static const ProgramDtoList sampleList_ZeroItem;
	static const ProgramDtoList sampleList_OneItem;
	static const ProgramDtoList sampleList_MoreItem;

	void checkToProgramDtoList(const ProgramDtoList& sampleList);
	void checkFromProgramDtoList(const ProgramDtoList& sampleList);

	void checkFromProgramDtoList_zeroItem();
	void checkFromProgramDtoList_oneItem();
	void checkFromProgramDtoList_moreItem();
};
