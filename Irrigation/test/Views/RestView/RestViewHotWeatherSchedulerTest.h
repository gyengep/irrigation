#pragma once
#include <memory>
#include <gmock/gmock.h>
#include "Logic/IdType.h"
#include "Mocks/MockProgramContainer.h"
#include "Mocks/MockProgram.h"
#include "Mocks/MockSchedulerContainer.h"
#include "Mocks/MockHotWeatherScheduler.h"
#include "Samples/HotWeatherSchedulerSamples.h"
#include "RestViewTest.h"


class RestViewHotWeatherSchedulerTest : public RestViewTest {
protected:

	static const IdType programId;
	static const RestViewTestSamples::HotWeatherSchedulerSample sample;

	std::shared_ptr<MockProgramContainer> mockProgramContainer;
	std::shared_ptr<MockProgram> mockProgram;
	std::shared_ptr<MockSchedulerContainer> mockSchedulerContainer;
	std::shared_ptr<MockHotWeatherScheduler> mockHotWeatherScheduler;

    virtual void SetUp();
    virtual void TearDown();

    static std::string createHotWeatherSchedulerUrl(IdType programId);
};
