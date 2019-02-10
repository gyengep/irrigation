#pragma once
#include <gmock/gmock.h>
#include "Logic/Program.h"



class MockProgram : public Program {
public:

	void setRunTimes(std::shared_ptr<RunTimeContainer> runTimes) { Program::runTimes = runTimes; }

	MOCK_CONST_METHOD1(isScheduled, bool(const std::tm& timeinfo));
	MOCK_CONST_METHOD0(getCurrentScheduler, const Scheduler&());

	bool programIsScheduled(const std::tm& timeinfo) const { return Program::isScheduled(timeinfo); }
};
