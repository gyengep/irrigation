#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "schedulers/Impl/WeeklySchedulerImpl.h"


class WeeklySchedulerImplTest : public ::testing::Test {
protected:

	std::shared_ptr<WeeklySchedulerImpl> weeklyScheduler;

    virtual void SetUp();
    virtual void TearDown();

    void checkDay(int year, int month, int day, bool expectedResult);
};

///////////////////////////////////////////////////////////////////////////////

class WeeklySchedulerImplDtoTest : public WeeklySchedulerImplTest {
protected:

	static const std::list<bool> originalEnabled;
	static const std::list<bool> newEnabled;

    virtual void SetUp();
    virtual void TearDown();
};
