#include "Schedulers/HotWeatherSchedulerImpl.h"
#include "Dto2Object/HotWeatherSchedulerSamples.h"
#include "Mocks/MockTemperatureHistory.h"
#include <gmock/gmock.h>
#include <memory>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(HotWeatherSchedulerImplToDtoTest, toHotWeatherSchedulerDto) {
	const Dto2ObjectTest::HotWeatherSchedulerSampleList sampleList;

	ASSERT_THAT(sampleList, SizeIs(4));

	for (const auto& sample : sampleList) {
		const HotWeatherSchedulerImpl& actual = sample.getObject();
		const HotWeatherSchedulerDTO& expected = sample.getDto();

		EXPECT_THAT(actual.toHotWeatherSchedulerDto(), Eq(expected));
	}
}

///////////////////////////////////////////////////////////////////////////////

enum class HotWeatherSchedulerDtoUpdateType {
	Nothing,
	Period,
	MinTemperature,
	All
};

void check(const HotWeatherSchedulerDtoUpdateType updateType, unsigned periodInSeconds, float minTemperature) {

	HotWeatherSchedulerImpl actualHotWeatherScheduler(
		std::make_shared<MockTemperatureHistory>(),
		std::chrono::seconds(periodInSeconds),
		minTemperature
	);

	unsigned expectedPeriodInSeconds = periodInSeconds;
	float expectedMinTemperature = minTemperature;

	for (const auto& sample : Dto2ObjectTest::HotWeatherSchedulerSampleList()) {
		HotWeatherSchedulerDTO actualHotWeatherSchedulerDTO;

		if (HotWeatherSchedulerDtoUpdateType::Period == updateType || HotWeatherSchedulerDtoUpdateType::All == updateType) {
			expectedPeriodInSeconds = sample.getDto().getPeriodInSeconds();
			actualHotWeatherSchedulerDTO.setPeriodInSeconds(expectedPeriodInSeconds);
		}

		if (HotWeatherSchedulerDtoUpdateType::MinTemperature == updateType || HotWeatherSchedulerDtoUpdateType::All == updateType) {
			expectedMinTemperature = sample.getDto().getMinTemperature();
			actualHotWeatherSchedulerDTO.setMinTemperature(expectedMinTemperature);
		}

		actualHotWeatherScheduler.updateFromHotWeatherSchedulerDto(actualHotWeatherSchedulerDTO);

		EXPECT_THAT(actualHotWeatherScheduler.getPeriod(), Eq(expectedPeriodInSeconds));
		EXPECT_THAT(actualHotWeatherScheduler.getMinTemperature(), Eq(expectedMinTemperature));
	}
}

TEST(HotWeatherSchedulerImplFromDtoTest, updateFromHotWeatherSchedulerDto_empty) {
	for (const auto& sample : Dto2ObjectTest::HotWeatherSchedulerSampleList()) {
		const HotWeatherSchedulerImpl& actual = sample.getObject();

		check(HotWeatherSchedulerDtoUpdateType::Nothing,
				actual.getPeriod(),
				actual.getMinTemperature()
			);
	}
}

TEST(HotWeatherSchedulerImplFromDtoTest, updateFromHotWeatherSchedulerDto_partial_minTemperature) {
	for (const auto& sample : Dto2ObjectTest::HotWeatherSchedulerSampleList()) {
		const HotWeatherSchedulerImpl& actual = sample.getObject();

		check(HotWeatherSchedulerDtoUpdateType::MinTemperature,
				actual.getPeriod(),
				actual.getMinTemperature()
			);
	}
}

TEST(HotWeatherSchedulerImplFromDtoTest, updateFromHotWeatherSchedulerDto_partial_period) {
	for (const auto& sample : Dto2ObjectTest::HotWeatherSchedulerSampleList()) {
		const HotWeatherSchedulerImpl& actual = sample.getObject();

		check(HotWeatherSchedulerDtoUpdateType::Period,
				actual.getPeriod(),
				actual.getMinTemperature()
			);
	}
}

TEST(HotWeatherSchedulerImplFromDtoTest, updateFromHotWeatherSchedulerDto_all) {
	for (const auto& sample : Dto2ObjectTest::HotWeatherSchedulerSampleList()) {
		const HotWeatherSchedulerImpl& actual = sample.getObject();

		check(HotWeatherSchedulerDtoUpdateType::All,
				actual.getPeriod(),
				actual.getMinTemperature()
			);
	}
}