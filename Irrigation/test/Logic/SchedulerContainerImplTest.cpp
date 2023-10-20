#include "SchedulerContainerImplTest.h"
#include "Exceptions/Exceptions.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(SchedulerContainerImplFactoryTest, create) {
	auto mockEveryDayScheduler = std::make_shared<StrictMock<MockEveryDayScheduler>>();
	auto mockHotWeatherScheduler = std::make_shared<StrictMock<MockHotWeatherScheduler>>();
	auto mockTemperatureDependentScheduler = std::make_shared<StrictMock<MockTemperatureDependentScheduler>>();
	auto mockWeeklyScheduler = std::make_shared<StrictMock<MockWeeklyScheduler>>();

	auto mockEveryDaySchedulerFactory = std::make_shared<StrictMock<MockEveryDaySchedulerFactory>>();
	auto mockHotWeatherSchedulerFactory = std::make_shared<StrictMock<MockHotWeatherSchedulerFactory>>();
	auto mockTemperatureDependentSchedulerFactory = std::make_shared<StrictMock<MockTemperatureDependentSchedulerFactory>>();
	auto mockWeeklySchedulerFactory = std::make_shared<StrictMock<MockWeeklySchedulerFactory>>();

	EXPECT_CALL(*mockEveryDaySchedulerFactory, create()).
			Times(1).
			WillOnce(Return(mockEveryDayScheduler));

	EXPECT_CALL(*mockHotWeatherSchedulerFactory, create()).
			Times(1).
			WillOnce(Return(mockHotWeatherScheduler));

	EXPECT_CALL(*mockTemperatureDependentSchedulerFactory, create()).
			Times(1).
			WillOnce(Return(mockTemperatureDependentScheduler));

	EXPECT_CALL(*mockWeeklySchedulerFactory, create()).
			Times(1).
			WillOnce(Return(mockWeeklyScheduler));

	SchedulerContainerPtr schedulerContainer = SchedulerContainerImplFactory(
			mockEveryDaySchedulerFactory,
			mockHotWeatherSchedulerFactory,
			mockTemperatureDependentSchedulerFactory,
			mockWeeklySchedulerFactory
		).create();

	ASSERT_THAT(schedulerContainer, Not(IsNull()));

	EXPECT_THAT(&schedulerContainer->getEveryDayScheduler(), Eq(mockEveryDayScheduler.get()));
	EXPECT_THAT(&schedulerContainer->getHotWeatherScheduler(), Eq(mockHotWeatherScheduler.get()));
	EXPECT_THAT(&schedulerContainer->getTemperatureDependentScheduler(), Eq(mockTemperatureDependentScheduler.get()));
	EXPECT_THAT(&schedulerContainer->getWeeklyScheduler(), Eq(mockWeeklyScheduler.get()));
}

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
