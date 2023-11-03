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

class ProgramImplDtoTest : public ProgramImplTest {
protected:

	static const bool originalEnabled = true;
	static const std::string originalName;
	static const unsigned originalAdjustment = 38;
	static const SchedulerType originalSchedulerType = SchedulerType::HOT_WEATHER;

	static const bool newEnabled = false;
	static const std::string newName;
	static const unsigned newAdjustment = 97;
	static const SchedulerType newSchedulerType = SchedulerType::TEMPERATURE_DEPENDENT;

	static const SchedulersDto schedulersDtoSample;
	static const RunTimeDtoList runTimeDtoListSample;
	static const StartTimeDtoList startTimeDtoListSample;

    virtual void SetUp();
    virtual void TearDown();
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
