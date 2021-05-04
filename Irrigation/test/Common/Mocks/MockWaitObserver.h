#pragma once
#include <gmock/gmock.h>
#include "Utils/WaitObserver.h"


class MockWaitObserver : public WaitObserver {
public:
	MOCK_METHOD1(wait_for, void(const std::chrono::milliseconds& ms));
	MOCK_METHOD1(wait_for_pred, void(const std::chrono::milliseconds& ms));
	MOCK_METHOD0(wait, void());
	MOCK_METHOD0(wait_pred, void());
};
