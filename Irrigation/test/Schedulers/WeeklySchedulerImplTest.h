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

class WeeklySchedulerImplFromDtoTest : public WeeklySchedulerImplTest {
protected:

	enum class UpdateType {
		Nothing,
		Days,
		All
	};

   	void checkUpdateFromWeeklySchedulerDto(const UpdateType updateType);
};
