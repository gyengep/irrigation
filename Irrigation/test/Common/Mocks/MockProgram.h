#pragma once
#include <gmock/gmock.h>
#include "Logic/Program.h"



class MockProgram : public Program {
public:

	void setRunTimes(std::shared_ptr<RunTimeContainer> runTimes) { Program::runTimes = runTimes; }

	MOCK_METHOD1(updateFromProgramDto, void(const ProgramDTO& programDTO));
	MOCK_METHOD0(getCurrentScheduler, Scheduler&());
};
