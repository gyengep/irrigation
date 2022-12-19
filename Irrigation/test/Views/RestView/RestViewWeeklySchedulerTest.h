#pragma once
#include <memory>
#include <gmock/gmock.h>
#include "Logic/IdType.h"
#include "Mocks/MockProgramContainer.h"
#include "Mocks/MockProgram.h"
#include "Mocks/MockSchedulerContainer.h"
#include "Mocks/MockWeeklyScheduler.h"
#include "Samples/WeeklySchedulerSamples.h"
#include "RestViewTest.h"


class RestViewWeeklySchedulerTest : public RestViewTest {
protected:

	static const IdType programId;
	static const Dto2XmlTest::WeeklySchedulerSample sample;

	std::shared_ptr<MockProgramContainer> mockProgramContainer;
	std::shared_ptr<MockProgram> mockProgram;
	std::shared_ptr<MockSchedulerContainer> mockSchedulerContainer;
	std::shared_ptr<MockWeeklyScheduler> mockWeeklyScheduler;

    virtual void SetUp();
    virtual void TearDown();

    static std::string createWeeklySchedulerUrl(IdType programId);
};
