#include "SchedulerContainerImplTest.h"
#include "Dto2ObjectSamples/SchedulerContainerSamples.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(SchedulerContainerImplToDtoTest, toSchedulerContainerDto) {
	const Dto2ObjectTestSamples::SchedulerContainerSampleList sampleList;

	ASSERT_THAT(sampleList, SizeIs(4));

	for (const auto& sample : sampleList) {
		const SchedulerContainerImpl& actual = sample.getObject();
		const SchedulersDTO& expected = sample.getDto();

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
		SchedulersDTO actualSchedulersDTO;

		if (UpdateType::EveryDayScheduler == updateType || UpdateType::All == updateType) {
			const EveryDaySchedulerDTO expectedEveryDaySchedulerDTO = sample.getDto().getEveryDayScheduler();

			actualSchedulersDTO.setEveryDayScheduler(EveryDaySchedulerDTO(expectedEveryDaySchedulerDTO));
			EXPECT_CALL(*mockEveryDayScheduler, updateFromEveryDaySchedulerDto(expectedEveryDaySchedulerDTO));
		}

		if (UpdateType::HotWeatherScheduler == updateType || UpdateType::All == updateType) {
			const HotWeatherSchedulerDTO expectedHotWeatherSchedulerDTO = sample.getDto().getHotWeatherScheduler();

			actualSchedulersDTO.setHotWeatherScheduler(HotWeatherSchedulerDTO(expectedHotWeatherSchedulerDTO));
			EXPECT_CALL(*mockHotWeatherScheduler, updateFromHotWeatherSchedulerDto(expectedHotWeatherSchedulerDTO));
		}

		if (UpdateType::TemperatureDependentScheduler == updateType || UpdateType::All == updateType) {
			const TemperatureDependentSchedulerDTO expectedTemperatureDependentSchedulerDTO = sample.getDto().getTemperatureDependentScheduler();

			actualSchedulersDTO.setTemperatureDependentScheduler(TemperatureDependentSchedulerDTO(expectedTemperatureDependentSchedulerDTO));
			EXPECT_CALL(*mockTemperatureDependentScheduler, updateFromTemperatureDependentSchedulerDto(expectedTemperatureDependentSchedulerDTO));
		}

		if (UpdateType::WeeklyScheduler == updateType || UpdateType::All == updateType) {
			const WeeklySchedulerDTO expectedWeeklySchedulerDTO = sample.getDto().getWeeklyScheduler();

			actualSchedulersDTO.setWeeklyScheduler(WeeklySchedulerDTO(expectedWeeklySchedulerDTO));
			EXPECT_CALL(*mockWeeklyScheduler, updateFromWeeklySchedulerDto(expectedWeeklySchedulerDTO));
		}

		actualSchedulerContainer.updateFromSchedulersDto(actualSchedulersDTO);
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
