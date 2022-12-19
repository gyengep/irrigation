#pragma once
#include <memory>
#include <gmock/gmock.h>
#include "Logic/IdType.h"
#include "Mocks/MockProgramContainer.h"
#include "Mocks/MockProgram.h"
#include "Mocks/MockStartTimeContainer.h"
#include "Mocks/MockStartTime.h"
#include "Samples/StartTimeSamples.h"
#include "RestViewTest.h"


class RestViewStartTimeTest : public RestViewTest {
public:

	static const IdType programId;
	static const IdType startTimeId;
	static const Dto2XmlTest::StartTimeSample sample;

	std::shared_ptr<MockProgramContainer> mockProgramContainer;
	std::shared_ptr<MockProgram> mockProgram;
	std::shared_ptr<MockStartTimeContainer> mockStartTimeContainer;
	std::shared_ptr<MockStartTime> mockStartTime;

    virtual void SetUp();
    virtual void TearDown();

    static std::string createStartTimeUrl(IdType programId, IdType startTimeId);
    static std::string createStartTimeLocation(IdType programId, IdType startTimeId);
};
