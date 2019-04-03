#pragma once
#include <gmock/gmock.h>
#include "Schedulers/WeeklyScheduler.h"


class MockWeeklyScheduler : public WeeklyScheduler {
public:
	MOCK_METHOD1(updateFromWeeklySchedulerDto, void(const WeeklySchedulerDTO&));
};

