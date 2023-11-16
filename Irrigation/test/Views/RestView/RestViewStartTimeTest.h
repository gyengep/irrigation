#pragma once
#include <memory>
#include <gmock/gmock.h>
#include "Logic/IdType.h"
#include "Mocks/MockProgramContainer.h"
#include "Mocks/MockProgram.h"
#include "Mocks/MockStartTimeContainer.h"
#include "Mocks/MockStartTime.h"
#include "DTO/StartTimeDto.h"
#include "Samples/SampleBase.h"
#include "RestViewTest.h"


class RestViewStartTimeTest : public RestViewTest {
public:

	typedef RestViewTestSamples::Sample<StartTimeDto> StartTimeSample;

	static const IdType programId;
	static const IdType startTimeId;
	static const StartTimeSample sample;

	std::shared_ptr<MockProgramContainer> mockProgramContainer;
	std::shared_ptr<MockProgram> mockProgram;
	std::shared_ptr<MockStartTimeContainer> mockStartTimeContainer;
	std::shared_ptr<MockStartTime> mockStartTime;

    virtual void SetUp();
    virtual void TearDown();
};
