#pragma once
#include <memory>
#include <gmock/gmock.h>
#include "Logic/IdType.h"
#include "Mocks/MockProgramContainer.h"
#include "Mocks/MockProgram.h"
#include "Mocks/MockRunTimeContainer.h"
#include "Samples/RunTimeContainerSamples.h"
#include "RestViewTest.h"


class RestViewRunTimeContainerTest : public RestViewTest {
protected:

	static const IdType programId;
	static const Dto2XmlTest::RunTimeContainerSample sample;

	std::shared_ptr<MockProgramContainer> mockProgramContainer;
	std::shared_ptr<MockProgram> mockProgram;
	std::shared_ptr<MockRunTimeContainer> mockRunTimeContainer;

    virtual void SetUp();
    virtual void TearDown();

    static std::string createRunTimeContainerUrl(IdType programId);
};
