#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "Mocks/MockEveryDayScheduler.h"
#include "Mocks/MockHotWeatherScheduler.h"
#include "Mocks/MockTemperatureDependentScheduler.h"
#include "Mocks/MockWeeklyScheduler.h"
#include "Logic/SchedulerContainerImpl.h"


class SchedulerContainerImplUpdateFromOrToDtoTest : public ::testing::Test {
protected:

	const HotWeatherSchedulerDTO expectedHotWeatherSchedulerDTO1 = HotWeatherSchedulerDTO(10, 20);
	const HotWeatherSchedulerDTO expectedHotWeatherSchedulerDTO2 = HotWeatherSchedulerDTO(25, 35);
	const TemperatureDependentSchedulerDTO expectedTemperatureDependentSchedulerDto1 = TemperatureDependentSchedulerDTO(0, 1, 2);
	const TemperatureDependentSchedulerDTO expectedTemperatureDependentSchedulerDto2 = TemperatureDependentSchedulerDTO(10, 20, 30);
	const WeeklySchedulerDTO expectedWeeklySchedulerDto1 = WeeklySchedulerDTO(std::list<bool> { true, true, false, false, true, false, false});
	const WeeklySchedulerDTO expectedWeeklySchedulerDto2 = WeeklySchedulerDTO(std::list<bool> { false, false, true, false, true, false, false});

	const SchedulersDTO expectedSchedulerContainerDto1 {
		EveryDaySchedulerDTO(),
		HotWeatherSchedulerDTO(expectedHotWeatherSchedulerDTO1),
		TemperatureDependentSchedulerDTO(expectedTemperatureDependentSchedulerDto1),
		WeeklySchedulerDTO(expectedWeeklySchedulerDto1)
	};

	const SchedulersDTO expectedSchedulerContainerDto2 {
		EveryDaySchedulerDTO(),
		HotWeatherSchedulerDTO(expectedHotWeatherSchedulerDTO2),
		TemperatureDependentSchedulerDTO(expectedTemperatureDependentSchedulerDto2),
		WeeklySchedulerDTO(expectedWeeklySchedulerDto2)
	};


	std::shared_ptr<MockEveryDayScheduler> mockEveryDayScheduler;
	std::shared_ptr<MockHotWeatherScheduler> mockHotWeatherScheduler;
	std::shared_ptr<MockTemperatureDependentScheduler> mockTemperatureDependentScheduler;
	std::shared_ptr<MockWeeklyScheduler> mockWeeklyScheduler;

	std::shared_ptr<SchedulerContainerImpl> schedulerContainer;

	virtual void SetUp();
    virtual void TearDown();
};
