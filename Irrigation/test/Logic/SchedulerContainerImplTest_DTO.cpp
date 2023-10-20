#include "SchedulerContainerImplTest.h"
#include "Dto2ObjectSamples/SchedulerContainerSamples.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(SchedulerContainerImplToDtoTest, toSchedulerContainerDto) {
	const Dto2ObjectTestSamples::SchedulerContainerSampleList sampleList;

	ASSERT_THAT(sampleList, SizeIs(4));

	for (const auto& sample : sampleList) {
		const SchedulerContainerImpl& actual = sample.getObject();
		const SchedulersDto& expected = sample.getDto();

		EXPECT_THAT(actual.toSchedulersDto(), Eq(expected));
	}
}

///////////////////////////////////////////////////////////////////////////////

void SchedulerContainerImplFromDtoTest::checkUpdateFromSchedulersDto(const UpdateType updateType) {

	std::shared_ptr<MockEveryDayScheduler> mockEveryDayScheduler = std::make_shared<StrictMock<MockEveryDayScheduler>>();
	std::shared_ptr<MockHotWeatherScheduler> mockHotWeatherScheduler = std::make_shared<StrictMock<MockHotWeatherScheduler>>();
	std::shared_ptr<MockTemperatureDependentScheduler> mockTemperatureDependentScheduler = std::make_shared<StrictMock<MockTemperatureDependentScheduler>>();
	std::shared_ptr<MockWeeklyScheduler> mockWeeklyScheduler = std::make_shared<StrictMock<MockWeeklyScheduler>>();

	SchedulerContainerImpl actualSchedulerContainer(
		mockEveryDayScheduler,
		mockHotWeatherScheduler,
		mockTemperatureDependentScheduler,
		mockWeeklyScheduler
	);

	for (const auto& sample : Dto2ObjectTestSamples::SchedulerContainerSampleList()) {
		SchedulersDto actualSchedulersDto;

		if (UpdateType::EveryDayScheduler == updateType || UpdateType::All == updateType) {
			const EveryDaySchedulerDto expectedEveryDaySchedulerDto = sample.getDto().getEveryDayScheduler();

			actualSchedulersDto.setEveryDayScheduler(EveryDaySchedulerDto(expectedEveryDaySchedulerDto));
			EXPECT_CALL(*mockEveryDayScheduler, updateFromEveryDaySchedulerDto(expectedEveryDaySchedulerDto));
		}

		if (UpdateType::HotWeatherScheduler == updateType || UpdateType::All == updateType) {
			const HotWeatherSchedulerDto expectedHotWeatherSchedulerDto = sample.getDto().getHotWeatherScheduler();

			actualSchedulersDto.setHotWeatherScheduler(HotWeatherSchedulerDto(expectedHotWeatherSchedulerDto));
			EXPECT_CALL(*mockHotWeatherScheduler, updateFromHotWeatherSchedulerDto(expectedHotWeatherSchedulerDto));
		}

		if (UpdateType::TemperatureDependentScheduler == updateType || UpdateType::All == updateType) {
			const TemperatureDependentSchedulerDto expectedTemperatureDependentSchedulerDto = sample.getDto().getTemperatureDependentScheduler();

			actualSchedulersDto.setTemperatureDependentScheduler(TemperatureDependentSchedulerDto(expectedTemperatureDependentSchedulerDto));
			EXPECT_CALL(*mockTemperatureDependentScheduler, updateFromTemperatureDependentSchedulerDto(expectedTemperatureDependentSchedulerDto));
		}

		if (UpdateType::WeeklyScheduler == updateType || UpdateType::All == updateType) {
			const WeeklySchedulerDto expectedWeeklySchedulerDto = sample.getDto().getWeeklyScheduler();

			actualSchedulersDto.setWeeklyScheduler(WeeklySchedulerDto(expectedWeeklySchedulerDto));
			EXPECT_CALL(*mockWeeklyScheduler, updateFromWeeklySchedulerDto(expectedWeeklySchedulerDto));
		}

		actualSchedulerContainer.updateFromSchedulersDto(actualSchedulersDto);
	}
}

TEST_F(SchedulerContainerImplFromDtoTest, updateFromSchedulersDto_empty) {
	checkUpdateFromSchedulersDto(UpdateType::Nothing);
}

TEST_F(SchedulerContainerImplFromDtoTest, updateFromSchedulersDto_partial_everyDayScheduler) {
	checkUpdateFromSchedulersDto(UpdateType::EveryDayScheduler);
}

TEST_F(SchedulerContainerImplFromDtoTest, updateFromSchedulersDto_partial_hotWeatherScheduler) {
	checkUpdateFromSchedulersDto(UpdateType::HotWeatherScheduler);
}

TEST_F(SchedulerContainerImplFromDtoTest, updateFromSchedulersDto_partial_temperatureDependentScheduler) {
	checkUpdateFromSchedulersDto(UpdateType::TemperatureDependentScheduler);
}

TEST_F(SchedulerContainerImplFromDtoTest, updateFromSchedulersDto_partial_weeklyScheduler) {
	checkUpdateFromSchedulersDto(UpdateType::WeeklyScheduler);
}

TEST_F(SchedulerContainerImplFromDtoTest, updateFromSchedulersDto_all) {
	checkUpdateFromSchedulersDto(UpdateType::All);
}
