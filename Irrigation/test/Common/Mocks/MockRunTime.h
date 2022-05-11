#pragma once
#include <gmock/gmock.h>
#include "Logic/RunTime.h"


class MockRunTime : public RunTime {
public:
	MOCK_CONST_METHOD0(getSeconds, unsigned());
	MOCK_CONST_METHOD0(getMilliSeconds, unsigned());
	MOCK_METHOD1(setSeconds, void(unsigned));
	MOCK_METHOD1(setMilliSeconds, void(unsigned));

	MOCK_CONST_METHOD0(toRunTimeDto, RunTimeDTO());
	MOCK_METHOD1(updateFromRunTimeDto, void(const RunTimeDTO&));
};

///////////////////////////////////////////////////////////////////////////////

class MockRunTimeFactory : public RunTimeFactory {
public:
	mutable std::vector<std::shared_ptr<MockRunTime>> mockRunTimes;

	virtual RunTimePtr create() const {
		auto result = mockRunTimes.front();
		mockRunTimes.erase(mockRunTimes.begin());
		return result;
	}
};

