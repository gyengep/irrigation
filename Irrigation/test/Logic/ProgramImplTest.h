#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "Logic/Impl/ProgramImpl.h"
#include "Mocks/MockSchedulerContainer.h"
#include "Mocks/MockRunTimeContainer.h"
#include "Mocks/MockStartTimeContainer.h"


class ProgramImplTest : public ::testing::Test {
protected:

	std::shared_ptr<ProgramImpl> program;

	std::shared_ptr<MockSchedulerContainer> mockSchedulerContainer;
	std::shared_ptr<MockRunTimeContainer> mockRunTimeContainer;
	std::shared_ptr<MockStartTimeContainer> mockStartTimeContainer;

    virtual void SetUp();
    virtual void TearDown();
};

///////////////////////////////////////////////////////////////////////////////

class ProgramImplFromDtoTest : public ProgramImplTest {
protected:

	enum class UpdateType {
		Nothing,
		Enabled,
		Name,
		Adjustment,
		SchedulerType,
		SchedulerContainer,
		RunTimeContainer,
		StartTimeContainer,
		All
	};

    void checkUpdateFromProgramDto(const UpdateType updateType);
};

///////////////////////////////////////////////////////////////////////////////

class ProgramImplScheduledTest : public ProgramImplTest {
protected:

	static const StartTimeContainer::container_type startTimeList;
	static const unsigned year = 2018;
	static const unsigned month = 5;
	static const unsigned day = 27;

    void checkAppropriateScheduler(const SchedulerType schedulerType);

    virtual void SetUp();
    virtual void TearDown();
};
