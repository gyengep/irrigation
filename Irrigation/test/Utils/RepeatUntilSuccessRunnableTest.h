#pragma once
#include <gmock/gmock.h>
#include <chrono>
#include <memory>
#include <vector>
#include "Utils/RepeatUntilSuccessRunnable.h"
#include "Mocks/MockRunnable.h"
#include "Mocks/MockWaitObserver.h"


class RepeatUntilSuccessRunnableTest : public ::testing::Test  {
public:

	std::shared_ptr<MockRunnable> mockRunnable;
	std::vector<std::chrono::milliseconds> waitTimes;
	std::shared_ptr<MockWaitObserver> mockWaitObserver;
	std::unique_ptr<RepeatUntilSuccessRunnable> repeatUntilSuccessRunnable;

	virtual void SetUp();
    virtual void TearDown();
};
