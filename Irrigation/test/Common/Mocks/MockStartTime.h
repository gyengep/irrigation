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
	std::shared_ptr<MockStartTime> getNext() const {
		if (nextIndex >= mockStartTimes.size()) {
			throw std::runtime_error("MockStartTimeFactory::getNext() invalid index: " + std::to_string(nextIndex));
		}

		return mockStartTimes[nextIndex++];
	}

public:
	std::vector<std::shared_ptr<MockStartTime>> mockStartTimes;
	mutable size_t nextIndex;

	MockStartTimeFactory(size_t size) :
		mockStartTimes(size),
		nextIndex(0)
	{
		for (size_t i = 0; i < mockStartTimes.size(); ++i) {
			mockStartTimes[i] = std::make_shared<testing::StrictMock<MockStartTime>>();
		}

		ON_CALL(*this, create()).WillByDefault(testing::Invoke(this, &MockStartTimeFactory::getNext));
	}

	MockStartTimeFactory(std::initializer_list<std::shared_ptr<MockStartTime>> initializer) :
		mockStartTimes(initializer),
		nextIndex(0)
	{
		ON_CALL(*this, create()).WillByDefault(testing::Invoke(this, &MockStartTimeFactory::getNext));
	}

	MOCK_CONST_METHOD0(create, StartTimePtr());
};
