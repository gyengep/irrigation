#pragma once
#include <memory>
#include <gmock/gmock.h>
#include "Logic/IdType.h"
#include "Mocks/MockProgramContainer.h"
#include "Mocks/MockProgram.h"
#include "Mocks/MockStartTimeContainer.h"
#include "Samples/StartTimeContainerSamples.h"
#include "RestViewTest.h"


class RestViewStartTimeContainerTest : public RestViewTest {
protected:

	static const IdType programId;
	static const RestViewTestSamples::StartTimeContainerSample sample;
	static const RestViewTestSamples::StartTimeSample startTimeSample;

	std::shared_ptr<MockProgramContainer> mockProgramContainer;
	std::shared_ptr<MockProgram> mockProgram;
	std::shared_ptr<MockStartTimeContainer> mockStartTimeContainer;

    virtual void SetUp();
    virtual void TearDown();

    static std::string createStartTimeContainerUrl(IdType programId);
};
