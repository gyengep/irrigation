#include <gmock/gmock.h>
#include "Logic/SchedulerContainerImplBuilder.h"
#include "SchedulerContainerImplTest.h"

using namespace testing;
using ::testing::Return;
using ::testing::AnyNumber;

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

///////////////////////////////////////////////////////////////////////////////

void SchedulerContainerImplUpdateFromOrToDtoTest::SetUp() {
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

void SchedulerContainerImplUpdateFromOrToDtoTest::TearDown() {
}


TEST_F(SchedulerContainerImplUpdateFromOrToDtoTest, toDto1) {
	EXPECT_CALL(*mockEveryDayScheduler, toEveryDaySchedulerDto()).Times(1).WillOnce(Return(EveryDaySchedulerDTO()));
	EXPECT_CALL(*mockHotWeatherScheduler, toHotWeatherSchedulerDto()).Times(1).WillOnce(Return(expectedHotWeatherSchedulerDTO1));
	EXPECT_CALL(*mockTemperatureDependentScheduler, toTemperatureDependentSchedulerDto()).Times(1).WillOnce(Return(expectedTemperatureDependentSchedulerDto1));
	EXPECT_CALL(*mockWeeklyScheduler, toWeeklySchedulerDto()).Times(1).WillOnce(Return(expectedWeeklySchedulerDto1));

	EXPECT_THAT(schedulerContainer->toSchedulersDto(), Eq(expectedSchedulerContainerDto1));
}

TEST_F(SchedulerContainerImplUpdateFromOrToDtoTest, toDto2) {
	EXPECT_CALL(*mockEveryDayScheduler, toEveryDaySchedulerDto()).Times(1).WillOnce(Return(EveryDaySchedulerDTO()));
	EXPECT_CALL(*mockHotWeatherScheduler, toHotWeatherSchedulerDto()).Times(1).WillOnce(Return(expectedHotWeatherSchedulerDTO2));
	EXPECT_CALL(*mockTemperatureDependentScheduler, toTemperatureDependentSchedulerDto()).Times(1).WillOnce(Return(expectedTemperatureDependentSchedulerDto2));
	EXPECT_CALL(*mockWeeklyScheduler, toWeeklySchedulerDto()).Times(1).WillOnce(Return(expectedWeeklySchedulerDto2));

	EXPECT_THAT(schedulerContainer->toSchedulersDto(), Eq(expectedSchedulerContainerDto2));
}


TEST_F(SchedulerContainerImplUpdateFromOrToDtoTest, fromDto_empty) {
	schedulerContainer->updateFromSchedulersDto(SchedulersDTO());
}

TEST_F(SchedulerContainerImplUpdateFromOrToDtoTest, fromDto_partial_HotWeatherScheduler) {
	EXPECT_CALL(*mockHotWeatherScheduler, updateFromHotWeatherSchedulerDto(expectedHotWeatherSchedulerDTO1)).Times(1);
	EXPECT_CALL(*mockHotWeatherScheduler, updateFromHotWeatherSchedulerDto(expectedHotWeatherSchedulerDTO2)).Times(1);

	schedulerContainer->updateFromSchedulersDto(SchedulersDTO().setHotWeatherScheduler(HotWeatherSchedulerDTO(expectedHotWeatherSchedulerDTO1)));
	schedulerContainer->updateFromSchedulersDto(SchedulersDTO().setHotWeatherScheduler(HotWeatherSchedulerDTO(expectedHotWeatherSchedulerDTO2)));
}

TEST_F(SchedulerContainerImplUpdateFromOrToDtoTest, fromDto_partial_TemperatureDependentScheduler) {
	EXPECT_CALL(*mockTemperatureDependentScheduler, updateFromTemperatureDependentSchedulerDto(expectedTemperatureDependentSchedulerDto1)).Times(1);
	EXPECT_CALL(*mockTemperatureDependentScheduler, updateFromTemperatureDependentSchedulerDto(expectedTemperatureDependentSchedulerDto2)).Times(1);

	schedulerContainer->updateFromSchedulersDto(SchedulersDTO().setTemperatureDependentScheduler(TemperatureDependentSchedulerDTO(expectedTemperatureDependentSchedulerDto1)));
	schedulerContainer->updateFromSchedulersDto(SchedulersDTO().setTemperatureDependentScheduler(TemperatureDependentSchedulerDTO(expectedTemperatureDependentSchedulerDto2)));
}

TEST_F(SchedulerContainerImplUpdateFromOrToDtoTest, fromDto_partial_WeeklyScheduler) {
	EXPECT_CALL(*mockWeeklyScheduler, updateFromWeeklySchedulerDto(expectedWeeklySchedulerDto1)).Times(1);
	EXPECT_CALL(*mockWeeklyScheduler, updateFromWeeklySchedulerDto(expectedWeeklySchedulerDto2)).Times(1);

	schedulerContainer->updateFromSchedulersDto(SchedulersDTO().setWeeklyScheduler(WeeklySchedulerDTO(expectedWeeklySchedulerDto1)));
	schedulerContainer->updateFromSchedulersDto(SchedulersDTO().setWeeklyScheduler(WeeklySchedulerDTO(expectedWeeklySchedulerDto2)));
}

TEST_F(SchedulerContainerImplUpdateFromOrToDtoTest, fromDto_all) {
	EXPECT_CALL(*mockEveryDayScheduler, updateFromEveryDaySchedulerDto(EveryDaySchedulerDTO())).Times(2);
	EXPECT_CALL(*mockHotWeatherScheduler, updateFromHotWeatherSchedulerDto(expectedHotWeatherSchedulerDTO1)).Times(1);
	EXPECT_CALL(*mockHotWeatherScheduler, updateFromHotWeatherSchedulerDto(expectedHotWeatherSchedulerDTO2)).Times(1);
	EXPECT_CALL(*mockTemperatureDependentScheduler, updateFromTemperatureDependentSchedulerDto(expectedTemperatureDependentSchedulerDto1)).Times(1);
	EXPECT_CALL(*mockTemperatureDependentScheduler, updateFromTemperatureDependentSchedulerDto(expectedTemperatureDependentSchedulerDto2)).Times(1);
	EXPECT_CALL(*mockWeeklyScheduler, updateFromWeeklySchedulerDto(expectedWeeklySchedulerDto1)).Times(1);
	EXPECT_CALL(*mockWeeklyScheduler, updateFromWeeklySchedulerDto(expectedWeeklySchedulerDto2)).Times(1);

	schedulerContainer->updateFromSchedulersDto(expectedSchedulerContainerDto1);
	schedulerContainer->updateFromSchedulersDto(expectedSchedulerContainerDto2);
}
