#pragma once

#include <memory>

#include "Logic/Containers.h"
#include "Logic/ContainerFactories.h"


class RunTimeContainerTest : public ::testing::Test {
protected:

	std::shared_ptr<RunTimeContainer> runTimes;

	RunTimeContainerTest() : runTimes(RunTimeContainerFactoryHolder::create()) {}
    virtual ~RunTimeContainerTest() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};


class StartTimeContainerTest : public ::testing::Test {
protected:

	std::shared_ptr<StartTimeContainer> startTimes;

	StartTimeContainerTest() : startTimes(StartTimeContainerFactoryHolder::create()) {}
    virtual ~StartTimeContainerTest() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};


class ProgramContainerTest : public ::testing::Test {
protected:

	std::shared_ptr<ProgramContainer> programs;

	ProgramContainerTest() : programs(ProgramContainerFactoryHolder::create()) {}
    virtual ~ProgramContainerTest() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};
