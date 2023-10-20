#include "Schedulers/Impl/HotWeatherSchedulerImpl.h"
#include "Dto2ObjectSamples/HotWeatherSchedulerSamples.h"
#include "Mocks/MockTemperatureHistory.h"
#include <gmock/gmock.h>
#include <memory>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(HotWeatherSchedulerImplToDtoTest, toHotWeatherSchedulerDto) {
	const Dto2ObjectTestSamples::HotWeatherSchedulerSampleList sampleList;

	ASSERT_THAT(sampleList, SizeIs(4));

	for (const auto& sample : sampleList) {
		const HotWeatherSchedulerImpl& actual = sample.getObject();
		const HotWeatherSchedulerDto& expected = sample.getDto();

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

	for (const auto& sample : Dto2ObjectTestSamples::HotWeatherSchedulerSampleList()) {
		HotWeatherSchedulerDto actualHotWeatherSchedulerDto;

		if (HotWeatherSchedulerDtoUpdateType::Period == updateType || HotWeatherSchedulerDtoUpdateType::All == updateType) {
			expectedPeriodInSeconds = sample.getDto().getPeriodInSeconds();
			actualHotWeatherSchedulerDto.setPeriodInSeconds(expectedPeriodInSeconds);
		}

		if (HotWeatherSchedulerDtoUpdateType::MinTemperature == updateType || HotWeatherSchedulerDtoUpdateType::All == updateType) {
			expectedMinTemperature = sample.getDto().getMinTemperature();
			actualHotWeatherSchedulerDto.setMinTemperature(expectedMinTemperature);
		}

		actualHotWeatherScheduler.updateFromHotWeatherSchedulerDto(actualHotWeatherSchedulerDto);

		EXPECT_THAT(actualHotWeatherScheduler.getPeriod(), Eq(expectedPeriodInSeconds));
		EXPECT_THAT(actualHotWeatherScheduler.getMinTemperature(), Eq(expectedMinTemperature));
	}
}

TEST(HotWeatherSchedulerImplFromDtoTest, updateFromHotWeatherSchedulerDto_empty) {
	for (const auto& sample : Dto2ObjectTestSamples::HotWeatherSchedulerSampleList()) {
		const HotWeatherSchedulerImpl& actual = sample.getObject();

		check(HotWeatherSchedulerDtoUpdateType::Nothing,
				actual.getPeriod(),
				actual.getMinTemperature()
			);
	}
}

TEST(HotWeatherSchedulerImplFromDtoTest, updateFromHotWeatherSchedulerDto_partial_minTemperature) {
	for (const auto& sample : Dto2ObjectTestSamples::HotWeatherSchedulerSampleList()) {
		const HotWeatherSchedulerImpl& actual = sample.getObject();

		check(HotWeatherSchedulerDtoUpdateType::MinTemperature,
				actual.getPeriod(),
				actual.getMinTemperature()
			);
	}
}

TEST(HotWeatherSchedulerImplFromDtoTest, updateFromHotWeatherSchedulerDto_partial_period) {
	for (const auto& sample : Dto2ObjectTestSamples::HotWeatherSchedulerSampleList()) {
		const HotWeatherSchedulerImpl& actual = sample.getObject();

		check(HotWeatherSchedulerDtoUpdateType::Period,
				actual.getPeriod(),
				actual.getMinTemperature()
			);
	}
}

TEST(HotWeatherSchedulerImplFromDtoTest, updateFromHotWeatherSchedulerDto_all) {
	for (const auto& sample : Dto2ObjectTestSamples::HotWeatherSchedulerSampleList()) {
		const HotWeatherSchedulerImpl& actual = sample.getObject();

		check(HotWeatherSchedulerDtoUpdateType::All,
				actual.getPeriod(),
				actual.getMinTemperature()
			);
	}
}
