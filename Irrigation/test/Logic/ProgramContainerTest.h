#pragma once
#include <list>
#include <memory>
#include <gmock/gmock.h>
#include "Logic/ProgramContainer.h"



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

    static ProgramList getAsProgramList(const std::shared_ptr<ProgramContainer> programs);
};
