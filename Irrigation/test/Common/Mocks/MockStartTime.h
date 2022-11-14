#pragma once
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
public:
	mutable std::vector<std::shared_ptr<MockStartTime>> mockStartTimes;
	mutable size_t index = 0;

	MockStartTimeFactory(unsigned size) {
		for (unsigned i = 0; i < size; i++) {
			mockStartTimes.emplace_back(std::make_shared<testing::StrictMock<MockStartTime>>());
		}
	}

	virtual StartTimePtr create() const {
		if (index >= mockStartTimes.size()) {
			throw std::runtime_error("MockStartTimeFactory no more item");
		}

		return mockStartTimes[index++];
	}
};
