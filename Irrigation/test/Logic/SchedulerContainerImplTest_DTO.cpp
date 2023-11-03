#include "SchedulerContainerImplTest.h"

using namespace testing;

const EveryDaySchedulerDto SchedulerContainerImplDtoTest::everyDaySchedulerDtoSample;
const HotWeatherSchedulerDto SchedulerContainerImplDtoTest::hotWeatherSchedulerDtoSample(97, 18.3f);
const TemperatureDependentSchedulerDto SchedulerContainerImplDtoTest::temperatureDependentSchedulerDtoSample(3.45f, 116, 117);
const WeeklySchedulerDto SchedulerContainerImplDtoTest::weeklySchedulerDtoSample(std::list<bool>{true, false, true, true, false, false, false});

///////////////////////////////////////////////////////////////////////////////

void SchedulerContainerImplDtoTest::SetUp() {
	SchedulerContainerImplTest::SetUp();

	ON_CALL(*mockEveryDayScheduler, toEveryDaySchedulerDto()).WillByDefault(Return(everyDaySchedulerDtoSample));
	ON_CALL(*mockHotWeatherScheduler, toHotWeatherSchedulerDto()).WillByDefault(Return(hotWeatherSchedulerDtoSample));
	ON_CALL(*mockTemperatureDependentScheduler, toTemperatureDependentSchedulerDto()).WillByDefault(Return(temperatureDependentSchedulerDtoSample));
	ON_CALL(*mockWeeklyScheduler, toWeeklySchedulerDto()).WillByDefault(Return(weeklySchedulerDtoSample));
}

void SchedulerContainerImplDtoTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(SchedulerContainerImplDtoTest, toSchedulerContainerDto) {
	const SchedulersDto expectedSchedulersDto(
			std::move(EveryDaySchedulerDto(everyDaySchedulerDtoSample)),
			std::move(HotWeatherSchedulerDto(hotWeatherSchedulerDtoSample)),
			std::move(TemperatureDependentSchedulerDto(temperatureDependentSchedulerDtoSample)),
			std::move(WeeklySchedulerDto(weeklySchedulerDtoSample))
		);

	EXPECT_CALL(*mockEveryDayScheduler, toEveryDaySchedulerDto()).Times(1);
	EXPECT_CALL(*mockHotWeatherScheduler, toHotWeatherSchedulerDto()).Times(1);
	EXPECT_CALL(*mockTemperatureDependentScheduler, toTemperatureDependentSchedulerDto()).Times(1);
	EXPECT_CALL(*mockWeeklyScheduler, toWeeklySchedulerDto()).Times(1);

	EXPECT_THAT(schedulerContainer->toSchedulersDto(), Eq(expectedSchedulersDto));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(SchedulerContainerImplDtoTest, updateFromSchedulersDto_empty) {
	SchedulersDto schedulersDto;

	schedulerContainer->updateFromSchedulersDto(schedulersDto);
}

TEST_F(SchedulerContainerImplDtoTest, updateFromSchedulersDto_everyDayScheduler) {
	SchedulersDto schedulersDto;
	schedulersDto.setEveryDayScheduler(EveryDaySchedulerDto(everyDaySchedulerDtoSample));

	EXPECT_CALL(*mockEveryDayScheduler, updateFromEveryDaySchedulerDto(everyDaySchedulerDtoSample)).Times(1);

	schedulerContainer->updateFromSchedulersDto(schedulersDto);
}

TEST_F(SchedulerContainerImplDtoTest, updateFromSchedulersDto_hotWeatherScheduler) {
	SchedulersDto schedulersDto;
	schedulersDto.setHotWeatherScheduler(HotWeatherSchedulerDto(hotWeatherSchedulerDtoSample));

	EXPECT_CALL(*mockHotWeatherScheduler, updateFromHotWeatherSchedulerDto(hotWeatherSchedulerDtoSample)).Times(1);

	schedulerContainer->updateFromSchedulersDto(schedulersDto);
}

TEST_F(SchedulerContainerImplDtoTest, updateFromSchedulersDto_temperatureDependentScheduler) {
	SchedulersDto schedulersDto;
	schedulersDto.setTemperatureDependentScheduler(TemperatureDependentSchedulerDto(temperatureDependentSchedulerDtoSample));

	EXPECT_CALL(*mockTemperatureDependentScheduler, updateFromTemperatureDependentSchedulerDto(temperatureDependentSchedulerDtoSample)).Times(1);

	schedulerContainer->updateFromSchedulersDto(schedulersDto);
}

TEST_F(SchedulerContainerImplDtoTest, updateFromSchedulersDto_weeklyScheduler) {
	SchedulersDto schedulersDto;
	schedulersDto.setWeeklyScheduler(WeeklySchedulerDto(weeklySchedulerDtoSample));

	EXPECT_CALL(*mockWeeklyScheduler, updateFromWeeklySchedulerDto(weeklySchedulerDtoSample)).Times(1);

	schedulerContainer->updateFromSchedulersDto(schedulersDto);
}

TEST_F(SchedulerContainerImplDtoTest, updateFromSchedulersDto_all) {
	const SchedulersDto schedulersDto(
			std::move(EveryDaySchedulerDto(everyDaySchedulerDtoSample)),
			std::move(HotWeatherSchedulerDto(hotWeatherSchedulerDtoSample)),
			std::move(TemperatureDependentSchedulerDto(temperatureDependentSchedulerDtoSample)),
			std::move(WeeklySchedulerDto(weeklySchedulerDtoSample))
		);

	EXPECT_CALL(*mockEveryDayScheduler, updateFromEveryDaySchedulerDto(everyDaySchedulerDtoSample)).Times(1);
	EXPECT_CALL(*mockHotWeatherScheduler, updateFromHotWeatherSchedulerDto(hotWeatherSchedulerDtoSample)).Times(1);
	EXPECT_CALL(*mockTemperatureDependentScheduler, updateFromTemperatureDependentSchedulerDto(temperatureDependentSchedulerDtoSample)).Times(1);
	EXPECT_CALL(*mockWeeklyScheduler, updateFromWeeklySchedulerDto(weeklySchedulerDtoSample)).Times(1);

	schedulerContainer->updateFromSchedulersDto(schedulersDto);
}
