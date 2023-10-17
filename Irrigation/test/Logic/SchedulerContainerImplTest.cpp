#include "Logic/Impl/SchedulerContainerImpl.h"
#include "Mocks/MockEveryDayScheduler.h"
#include "Mocks/MockHotWeatherScheduler.h"
#include "Mocks/MockTemperatureDependentScheduler.h"
#include "Mocks/MockWeeklyScheduler.h"
#include <gmock/gmock.h>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(SchedulerContainerImplTest, defaultConstructor) {
	const auto mockEveryDayScheduler = std::make_shared<MockEveryDayScheduler>();
	const auto mockHotWeatherScheduler = std::make_shared<MockHotWeatherScheduler>();
	const auto mockTemperatureDependentScheduler = std::make_shared<MockTemperatureDependentScheduler>();
	const auto mockWeeklyScheduler = std::make_shared<MockWeeklyScheduler>();

	const SchedulerContainerImpl schedulerContainer(
		mockEveryDayScheduler,
		mockHotWeatherScheduler,
		mockTemperatureDependentScheduler,
		mockWeeklyScheduler
	);

	EXPECT_THAT(&schedulerContainer.getEveryDayScheduler(), Eq(mockEveryDayScheduler.get()));
	EXPECT_THAT(&schedulerContainer.getHotWeatherScheduler(), Eq(mockHotWeatherScheduler.get()));
	EXPECT_THAT(&schedulerContainer.getTemperatureDependentScheduler(), Eq(mockTemperatureDependentScheduler.get()));
	EXPECT_THAT(&schedulerContainer.getWeeklyScheduler(), Eq(mockWeeklyScheduler.get()));
}

TEST(SchedulerContainerImplTest, at) {
	const auto mockEveryDayScheduler = std::make_shared<MockEveryDayScheduler>();
	const auto mockHotWeatherScheduler = std::make_shared<MockHotWeatherScheduler>();
	const auto mockTemperatureDependentScheduler = std::make_shared<MockTemperatureDependentScheduler>();
	const auto mockWeeklyScheduler = std::make_shared<MockWeeklyScheduler>();

	const SchedulerContainerImpl schedulerContainer(
		mockEveryDayScheduler,
		mockHotWeatherScheduler,
		mockTemperatureDependentScheduler,
		mockWeeklyScheduler
	);

	EXPECT_THAT(schedulerContainer.at(SchedulerType::EVERY_DAY).get(), Eq(mockEveryDayScheduler.get()));
	EXPECT_THAT(schedulerContainer.at(SchedulerType::HOT_WEATHER).get(), Eq(mockHotWeatherScheduler.get()));
	EXPECT_THAT(schedulerContainer.at(SchedulerType::TEMPERATURE_DEPENDENT).get(), Eq(mockTemperatureDependentScheduler.get()));
	EXPECT_THAT(schedulerContainer.at(SchedulerType::WEEKLY).get(), Eq(mockWeeklyScheduler.get()));
}
