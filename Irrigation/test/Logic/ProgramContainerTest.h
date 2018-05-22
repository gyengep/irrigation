#pragma once
#include <list>
#include <memory>
#include <gmock/gmock.h>
#include "Logic/ProgramContainer.h"



class ProgramContainerTest : public ::testing::Test {
protected:

	typedef std::list<ProgramContainer::value_type> ProgramList;

	class ProgramContainerCallback {
		ProgramList programList;

	public:
		void callback(const IdType id, LockedProgramPtr lockedProgramPtr);
		const ProgramList& getProgramList() const;
	};

	ProgramContainer programs;

    virtual void SetUp();
    virtual void TearDown();

    static ProgramList getAsProgramList(const ProgramContainer* programs);
};
