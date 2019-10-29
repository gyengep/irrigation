#pragma once
#include <gmock/gmock.h>
#include "Schedulers/Scheduler.h"


class MockScheduler : public Scheduler {
public:
	MOCK_METHOD1(process, Scheduler::Result(const std::time_t rawtime));
};
