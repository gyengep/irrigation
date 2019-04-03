#pragma once
#include <gmock/gmock.h>
#include "Schedulers/PeriodicScheduler.h"


class MockPeriodicScheduler : public PeriodicScheduler {
public:
	MOCK_METHOD1(updateFromPeriodicSchedulerDto, void(const PeriodicSchedulerDTO&));
};

