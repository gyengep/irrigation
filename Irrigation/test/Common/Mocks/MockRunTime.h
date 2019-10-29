#pragma once
#include <gmock/gmock.h>
#include "Logic/RunTime.h"


class MockRunTime : public RunTime {
public:
	MockRunTime() {
		EXPECT_CALL(*this, destroyed()).Times(1);
	}

	MOCK_METHOD1(updateFromRunTimeDto, void(const RunTimeDTO& runTimeDTO));
	MOCK_METHOD0(destroyed, void());
	virtual ~MockRunTime() { destroyed(); }
};

///////////////////////////////////////////////////////////////////////////////

class MockRunTimeFactory : public RunTimeFactory {
public:
	std::vector<std::shared_ptr<MockRunTime>> mockRunTimes;

	virtual std::shared_ptr<RunTime> createRunTime() {
		mockRunTimes.push_back(std::shared_ptr<MockRunTime>(new MockRunTime()));
		return mockRunTimes.back();
	}
};

