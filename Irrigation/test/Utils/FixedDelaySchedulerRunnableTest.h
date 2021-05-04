#pragma once
#include <gmock/gmock.h>
#include <chrono>
#include <memory>
#include <vector>
#include "Utils/FixedDelaySchedulerRunnable.h"
#include "Mocks/MockRunnable.h"
#include "Mocks/MockWaitObserver.h"


class FixedDelaySchedulerRunnableTest : public ::testing::Test  {
public:

	std::shared_ptr<MockWaitObserver> mockWaitObserver;
	std::shared_ptr<MockRunnable> mockRunnable;
	//std::shared_ptr<FixedDelaySchedulerThread> fixedDelaySchedulerThread;

	virtual void SetUp();
    virtual void TearDown();
};
