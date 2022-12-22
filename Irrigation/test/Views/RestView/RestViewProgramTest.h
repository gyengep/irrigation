#pragma once
#include <memory>
#include <gmock/gmock.h>
#include "Logic/IdType.h"
#include "Mocks/MockProgramContainer.h"
#include "Mocks/MockProgram.h"
#include "Mocks/MockProgramContainer.h"
#include "Mocks/MockProgram.h"
#include "Samples/ProgramSamples.h"
#include "RestViewTest.h"


class RestViewProgramTest : public RestViewTest {
public:

	static const IdType programId;
	static const RestViewTestSamples::ProgramSample sample;

	std::shared_ptr<MockProgramContainer> mockProgramContainer;
	std::shared_ptr<MockProgram> mockProgram;

    virtual void SetUp();
    virtual void TearDown();

    static std::string createProgramUrl(IdType programId);
    static std::string createProgramLocation(IdType programId);
};
