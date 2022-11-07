#pragma once
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
public:
	mutable std::vector<std::shared_ptr<MockRunTime>> mockRunTimes;
	mutable size_t index = 0;

	MockRunTimeFactory(unsigned size) {
		for (unsigned i = 0; i < size; i++) {
			mockRunTimes.emplace_back(std::make_shared<testing::StrictMock<MockRunTime>>());
		}
	}

	virtual RunTimePtr create() const {
		if (index >= mockRunTimes.size()) {
			throw std::runtime_error("MockRunTimeFactory no more item");
		}

		return mockRunTimes[index++];
	}
};
