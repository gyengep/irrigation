#pragma once
#include <gmock/gmock.h>
#include "Logic/Program.h"



class MockProgram : public Program {
public:

	void setRunTimes(std::shared_ptr<RunTimeContainer> runTimes) { Program::runTimes = runTimes; }

	MOCK_METHOD1(updateFromProgramDto, void(const ProgramDTO& programDTO));
	MOCK_METHOD1(isScheduled, bool(const std::time_t rawtime));
	MOCK_CONST_METHOD0(getCurrentScheduler, const Scheduler&());
	MOCK_METHOD0(getCurrentScheduler, Scheduler&());

	bool programIsScheduled(const std::time_t rawtime) { return Program::isScheduled(rawtime); }
};
