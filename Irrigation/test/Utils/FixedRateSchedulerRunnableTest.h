#pragma once
#include <gmock/gmock.h>
#include <chrono>
#include <memory>
#include <vector>
#include "Utils/FixedRateSchedulerRunnable.h"
#include "Mocks/MockRunnable.h"
#include "Mocks/MockWaitObserver.h"


class FixedRateSchedulerRunnableTest : public ::testing::Test  {
public:

	std::shared_ptr<MockWaitObserver> mockWaitObserver;
	std::shared_ptr<MockRunnable> mockRunnable;

	virtual void SetUp();
    virtual void TearDown();
};
