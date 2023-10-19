#include "SchedulerContainerImplTest.h"
#include "Exceptions/Exceptions.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void SchedulerContainerImplTest::SetUp() {
	mockEveryDayScheduler = std::make_shared<StrictMock<MockEveryDayScheduler>>();
	mockHotWeatherScheduler = std::make_shared<StrictMock<MockHotWeatherScheduler>>();
	mockTemperatureDependentScheduler = std::make_shared<StrictMock<MockTemperatureDependentScheduler>>();
	mockWeeklyScheduler = std::make_shared<StrictMock<MockWeeklyScheduler>>();

	schedulerContainer = std::make_shared<SchedulerContainerImpl>(
		mockEveryDayScheduler,
		mockHotWeatherScheduler,
		mockTemperatureDependentScheduler,
		mockWeeklyScheduler
	);
}

void SchedulerContainerImplTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(SchedulerContainerImplTest, defaultValues) {
	EXPECT_THAT(&schedulerContainer->getEveryDayScheduler(), Eq(mockEveryDayScheduler.get()));
	EXPECT_THAT(&schedulerContainer->getHotWeatherScheduler(), Eq(mockHotWeatherScheduler.get()));
	EXPECT_THAT(&schedulerContainer->getTemperatureDependentScheduler(), Eq(mockTemperatureDependentScheduler.get()));
	EXPECT_THAT(&schedulerContainer->getWeeklyScheduler(), Eq(mockWeeklyScheduler.get()));

	EXPECT_THAT(&Const(schedulerContainer)->getEveryDayScheduler(), Eq(mockEveryDayScheduler.get()));
	EXPECT_THAT(&Const(schedulerContainer)->getHotWeatherScheduler(), Eq(mockHotWeatherScheduler.get()));
	EXPECT_THAT(&Const(schedulerContainer)->getTemperatureDependentScheduler(), Eq(mockTemperatureDependentScheduler.get()));
	EXPECT_THAT(&Const(schedulerContainer)->getWeeklyScheduler(), Eq(mockWeeklyScheduler.get()));
}

TEST_F(SchedulerContainerImplTest, at) {
	EXPECT_THAT(schedulerContainer->at(SchedulerType::EVERY_DAY), Eq(mockEveryDayScheduler));
	EXPECT_THAT(schedulerContainer->at(SchedulerType::HOT_WEATHER), Eq(mockHotWeatherScheduler));
	EXPECT_THAT(schedulerContainer->at(SchedulerType::TEMPERATURE_DEPENDENT), Eq(mockTemperatureDependentScheduler));
	EXPECT_THAT(schedulerContainer->at(SchedulerType::WEEKLY), Eq(mockWeeklyScheduler));

	EXPECT_THAT(Const(schedulerContainer)->at(SchedulerType::EVERY_DAY), Eq(mockEveryDayScheduler));
	EXPECT_THAT(Const(schedulerContainer)->at(SchedulerType::HOT_WEATHER), Eq(mockHotWeatherScheduler));
	EXPECT_THAT(Const(schedulerContainer)->at(SchedulerType::TEMPERATURE_DEPENDENT), Eq(mockTemperatureDependentScheduler));
	EXPECT_THAT(Const(schedulerContainer)->at(SchedulerType::WEEKLY), Eq(mockWeeklyScheduler));
}
