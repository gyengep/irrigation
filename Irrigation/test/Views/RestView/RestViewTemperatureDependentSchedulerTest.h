#pragma once
#include <memory>
#include <gmock/gmock.h>
#include "Logic/IdType.h"
#include "Mocks/MockProgramContainer.h"
#include "Mocks/MockProgram.h"
#include "Mocks/MockSchedulerContainer.h"
#include "Mocks/MockTemperatureDependentScheduler.h"
#include "Samples/TemperatureDependentSchedulerSamples.h"
#include "RestViewTest.h"


class RestViewTemperatureDependentSchedulerTest : public RestViewTest {
protected:

	static const IdType programId;
	static const RestViewTestSamples::TemperatureDependentSchedulerSample sample;

	std::shared_ptr<MockProgramContainer> mockProgramContainer;
	std::shared_ptr<MockProgram> mockProgram;
	std::shared_ptr<MockSchedulerContainer> mockSchedulerContainer;
	std::shared_ptr<MockTemperatureDependentScheduler> mockTemperatureDependentScheduler;

    virtual void SetUp();
    virtual void TearDown();
};
