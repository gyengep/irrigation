#pragma once
#include <vector>
#include <gmock/gmock.h>
#include "Logic/RunTime.h"


class MockRunTime : public RunTime {
public:
	MOCK_CONST_METHOD0(getSeconds, unsigned());
	MOCK_METHOD1(setSeconds, void(unsigned));

	MOCK_CONST_METHOD0(toRunTimeDto, RunTimeDTO());
	MOCK_METHOD1(updateFromRunTimeDto, void(const RunTimeDTO&));

	MOCK_CONST_METHOD0(toString, std::string());
	MOCK_CONST_METHOD0(toDuration, std::chrono::seconds());
};

///////////////////////////////////////////////////////////////////////////////

class MockRunTimeFactory : public RunTimeFactory {
	std::shared_ptr<MockRunTime> getNext() const {
		if (nextIndex >= mockRunTimes.size()) {
			throw std::runtime_error("MockRunTimeFactory::getNext() invalid index: " + std::to_string(nextIndex));
		}

		return mockRunTimes[nextIndex++];
	}

public:
	std::vector<std::shared_ptr<MockRunTime>> mockRunTimes;
	mutable size_t nextIndex;

	MockRunTimeFactory(size_t size) :
		mockRunTimes(size),
		nextIndex(0)
	{
		for (size_t i = 0; i < mockRunTimes.size(); ++i) {
			mockRunTimes[i] = std::make_shared<testing::StrictMock<MockRunTime>>();
		}

		ON_CALL(*this, create()).WillByDefault(testing::Invoke(this, &MockRunTimeFactory::getNext));
	}

	MockRunTimeFactory(std::initializer_list<std::shared_ptr<MockRunTime>> initializer) :
		mockRunTimes(initializer),
		nextIndex(0)
	{
		ON_CALL(*this, create()).WillByDefault(testing::Invoke(this, &MockRunTimeFactory::getNext));
	}

	MOCK_CONST_METHOD0(create, RunTimePtr());
};
