#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "Logic/Impl/SchedulerContainerImpl.h"
#include "Mocks/MockEveryDayScheduler.h"
#include "Mocks/MockHotWeatherScheduler.h"
#include "Mocks/MockTemperatureDependentScheduler.h"
#include "Mocks/MockWeeklyScheduler.h"


class SchedulerContainerImplTest : public ::testing::Test {
protected:

	std::shared_ptr<SchedulerContainerImpl> schedulerContainer;

	std::shared_ptr<MockEveryDayScheduler> mockEveryDayScheduler;
	std::shared_ptr<MockHotWeatherScheduler> mockHotWeatherScheduler;
	std::shared_ptr<MockTemperatureDependentScheduler> mockTemperatureDependentScheduler;
	std::shared_ptr<MockWeeklyScheduler> mockWeeklyScheduler;

    virtual void SetUp();
    virtual void TearDown();
};

///////////////////////////////////////////////////////////////////////////////

class SchedulerContainerImplFromDtoTest : public SchedulerContainerImplTest {
protected:

	enum class UpdateType {
		Nothing,
		EveryDayScheduler,
		HotWeatherScheduler,
		TemperatureDependentScheduler,
		WeeklyScheduler,
		All
	};

    void checkUpdateFromSchedulersDto(const UpdateType updateType);
};
