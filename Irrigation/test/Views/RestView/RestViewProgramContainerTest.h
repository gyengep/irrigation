#pragma once
#include <memory>
#include <gmock/gmock.h>
#include "Logic/IdType.h"
#include "Mocks/MockProgramContainer.h"
#include "Samples/ProgramContainerSamples.h"
#include "RestViewTest.h"


class RestViewProgramContainerTest : public RestViewTest {
protected:

	static const Dto2XmlTest::ProgramContainerSample sample;
	static const Dto2XmlTest::ProgramSample programSample;

	std::shared_ptr<MockProgramContainer> mockProgramContainer;

    virtual void SetUp();
    virtual void TearDown();

    static std::string createProgramContainerUrl();
};
