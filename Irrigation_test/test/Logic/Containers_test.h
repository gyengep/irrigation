#pragma once

#include <memory>

#include "Logic/Containers.h"
#include "Logic/ContainerFactories.h"


class RunTimeContainerTest : public ::testing::Test {
protected:

	std::shared_ptr<RunTimeContainer> runTimes;

    virtual void SetUp();
    virtual void TearDown();
};


class StartTimeContainerTest : public ::testing::Test {
protected:

	typedef std::list<std::pair<IdType, StartTime>> StartTimeList;

	std::shared_ptr<StartTimeContainer> startTimes;

    virtual void SetUp();
    virtual void TearDown();

    static StartTimeList getAsStartTimeList(std::shared_ptr<StartTimeContainer> startTimes);
};


class ProgramContainerTest : public ::testing::Test {
protected:

	typedef std::list<std::pair<IdType, Program*>> ProgramList;

	class ProgramContainerCallback {
		ProgramList programList;

	public:
		void callback(IdType id, LockedProgram program);
		const ProgramList& getProgramList() const;
	};

	std::shared_ptr<ProgramContainer> programs;

    virtual void SetUp();
    virtual void TearDown();

    static ProgramList getAsProgramList(const ProgramContainer* programs);
    static void insertPrograms(ProgramContainer* programs, size_t count, std::vector<IdType>* result);
    static void erasePrograms(ProgramContainer* programs, std::vector<IdType>* ids);
    static void getProgramAndWait(ProgramContainer* programs, IdType id, int ms);
};
