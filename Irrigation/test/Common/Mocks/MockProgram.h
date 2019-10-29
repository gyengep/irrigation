#pragma once
#include <gmock/gmock.h>
#include "Logic/Program.h"


class MockProgram : public Program {
public:
	MOCK_METHOD1(updateFromProgramDto, void(const ProgramDTO& programDTO));
	MOCK_METHOD0(getCurrentScheduler, Scheduler&());
};
