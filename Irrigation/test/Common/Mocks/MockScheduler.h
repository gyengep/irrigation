#pragma once
#include <gmock/gmock.h>
#include "Schedulers/Scheduler.h"



class MockScheduler : public Scheduler {
public:
	MOCK_CONST_METHOD1(isDayScheduled, bool(const std::tm& timeinfo));
	MOCK_CONST_METHOD0(getAdjustment, unsigned());
};

