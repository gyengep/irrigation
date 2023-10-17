#include "Logic/Impl/SchedulerContainerImpl.h"
#include "Samples/SchedulerContainerSamples.h"
#include "Mocks/MockEveryDayScheduler.h"
#include "Mocks/MockHotWeatherScheduler.h"
#include "Mocks/MockTemperatureDependentScheduler.h"
#include "Mocks/MockWeeklyScheduler.h"
#include <gmock/gmock.h>
#include <memory>

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

enum class SchedulerContainerDtoUpdateType {
	Nothing,
	EveryDayScheduler,
	HotWeatherScheduler,
	TemperatureDependentScheduler,
	WeeklyScheduler,
	All
};

void check(const SchedulerContainerDtoUpdateType updateType) {

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

		if (SchedulerContainerDtoUpdateType::EveryDayScheduler == updateType || SchedulerContainerDtoUpdateType::All == updateType) {
			actualSchedulersDTO.setEveryDayScheduler(EveryDaySchedulerDTO(sample.getDto().getEveryDayScheduler()));
			EXPECT_CALL(*mockEveryDayScheduler, updateFromEveryDaySchedulerDto(sample.getObject().getEveryDayScheduler().toEveryDaySchedulerDto()));
		}

		if (SchedulerContainerDtoUpdateType::HotWeatherScheduler == updateType || SchedulerContainerDtoUpdateType::All == updateType) {
			actualSchedulersDTO.setHotWeatherScheduler(HotWeatherSchedulerDTO(sample.getDto().getHotWeatherScheduler()));
			EXPECT_CALL(*mockHotWeatherScheduler, updateFromHotWeatherSchedulerDto(sample.getObject().getHotWeatherScheduler().toHotWeatherSchedulerDto()));
		}

		if (SchedulerContainerDtoUpdateType::TemperatureDependentScheduler == updateType || SchedulerContainerDtoUpdateType::All == updateType) {
			actualSchedulersDTO.setTemperatureDependentScheduler(TemperatureDependentSchedulerDTO(sample.getDto().getTemperatureDependentScheduler()));
			EXPECT_CALL(*mockTemperatureDependentScheduler, updateFromTemperatureDependentSchedulerDto(sample.getObject().getTemperatureDependentScheduler().toTemperatureDependentSchedulerDto()));
		}

		if (SchedulerContainerDtoUpdateType::WeeklyScheduler == updateType || SchedulerContainerDtoUpdateType::All == updateType) {
			actualSchedulersDTO.setWeeklyScheduler(WeeklySchedulerDTO(sample.getDto().getWeeklyScheduler()));
			EXPECT_CALL(*mockWeeklyScheduler, updateFromWeeklySchedulerDto(sample.getObject().getWeeklyScheduler().toWeeklySchedulerDto()));
		}

		actualSchedulerContainer.updateFromSchedulersDto(actualSchedulersDTO);
	}
}

TEST(SchedulerContainerImplFromDtoTest, updateFromSchedulerContainerDto_empty) {
	check(SchedulerContainerDtoUpdateType::Nothing);
}

TEST(SchedulerContainerImplFromDtoTest, updateFromSchedulerContainerDto_partial_everyDayScheduler) {
	check(SchedulerContainerDtoUpdateType::EveryDayScheduler);
}

TEST(SchedulerContainerImplFromDtoTest, updateFromSchedulerContainerDto_partial_hotWeatherScheduler) {
	check(SchedulerContainerDtoUpdateType::HotWeatherScheduler);
}

TEST(SchedulerContainerImplFromDtoTest, updateFromSchedulerContainerDto_partial_temperatureDependentScheduler) {
	check(SchedulerContainerDtoUpdateType::TemperatureDependentScheduler);
}

TEST(SchedulerContainerImplFromDtoTest, updateFromSchedulerContainerDto_partial_weeklyScheduler) {
	check(SchedulerContainerDtoUpdateType::WeeklyScheduler);
}

TEST(SchedulerContainerImplFromDtoTest, updateFromSchedulerContainerDto_all) {
	check(SchedulerContainerDtoUpdateType::All);
}
