#pragma once
#include <gmock/gmock.h>
#include "Logic/StartTime.h"



class MockStartTime : public StartTime {
public:
	MockStartTime() {
		EXPECT_CALL(*this, destroyed()).Times(1);
	}

	MOCK_METHOD0(destroyed, void());
	virtual ~MockStartTime() { destroyed(); }
};
