#pragma once
#include <vector>
#include <gmock/gmock.h>
#include "Logic/StartTime.h"


class MockStartTime : public StartTime {
public:
	MOCK_CONST_METHOD1(less, bool(const StartTime& other));

	MOCK_METHOD2(set, void(unsigned hour, unsigned minute));

	MOCK_CONST_METHOD0(getHours, unsigned());
	MOCK_CONST_METHOD0(getMinutes, unsigned());

	MOCK_CONST_METHOD0(toStartTimeDto, StartTimeDTO());
	MOCK_METHOD1(updateFromStartTimeDto, void(const StartTimeDTO& startTimeDTO));

	MOCK_CONST_METHOD0(toString, std::string());
	MOCK_CONST_METHOD0(toShortString, std::string());
};

///////////////////////////////////////////////////////////////////////////////

class MockStartTimeFactory : public StartTimeFactory {
	unsigned idx = 0;

public:
	std::vector<std::shared_ptr<MockStartTime>> mockStartTimes;

	StartTimePtr createMockStartTime() {
		if (mockStartTimes.size() <= idx) {
			throw std::logic_error("MockStartTimeFactory::createMockStartTime()");
		}

		return mockStartTimes[idx++];
	}

	MOCK_CONST_METHOD0(create, StartTimePtr());
};
