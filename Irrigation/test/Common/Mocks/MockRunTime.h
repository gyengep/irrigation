#pragma once
#include <vector>
#include <gmock/gmock.h>
#include "Logic/RunTime.h"


class MockRunTime : public RunTime {
public:
	MOCK_CONST_METHOD0(get, std::chrono::seconds());
	MOCK_METHOD1(set, void(const std::chrono::seconds&));

	MOCK_CONST_METHOD0(toRunTimeDto, RunTimeDTO());
	MOCK_METHOD1(updateFromRunTimeDto, void(const RunTimeDTO&));

	MOCK_CONST_METHOD0(toString, std::string());
	MOCK_CONST_METHOD0(toDuration, std::chrono::seconds());
};

///////////////////////////////////////////////////////////////////////////////

class MockRunTimeFactory : public RunTimeFactory {
	unsigned idx = 0;

public:
	std::vector<std::shared_ptr<MockRunTime>> mockRunTimes;

	RunTimePtr createMockRunTime() {
		if (mockRunTimes.size() <= idx) {
			throw std::logic_error("MockRunTimeFactory::createMockRunTime()");
		}

		return mockRunTimes[idx++];
	}

	MOCK_CONST_METHOD0(create, RunTimePtr());
};
