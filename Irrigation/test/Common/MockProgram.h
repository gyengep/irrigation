#pragma once
#include <gmock/gmock.h>
#include "Logic/Program.h"



class MockProgram : public Program {
public:
	MockProgram() {
		EXPECT_CALL(*this, destroyed()).Times(1);
	}

	MOCK_METHOD0(destroyed, void());
	MOCK_CONST_METHOD1(isScheduled, bool(const std::tm& timeinfo));
	MOCK_CONST_METHOD0(getCurrentScheduler, Scheduler&());
	virtual ~MockProgram() { destroyed(); }
};

///////////////////////////////////////////////////////////////////////////////

class MockProgram_Scheduler : public Program {
public:
	MOCK_CONST_METHOD0(getCurrentScheduler, const Scheduler&());
};

