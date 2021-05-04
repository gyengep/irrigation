#pragma once
#include <gmock/gmock.h>
#include "Utils/SynchronizationObject.h"


class MockSynchronizationObject : public SynchronizationObject {
public:

	MOCK_METHOD1(updateFromStartTimeDto, void(const StartTimeDTO& startTimeDTO));
	MOCK_METHOD0(destroyed, void());
};
