#include "Schedulers/Impl/TemperatureDependentSchedulerImpl.h"
#include "Dto2ObjectSamples/TemperatureDependentSchedulerSamples.h"
#include "Mocks/MockTemperatureForecast.h"
#include "Mocks/MockTemperatureHistory.h"
#include <gmock/gmock.h>
#include <memory>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureDependentSchedulerImplToDtoTest, toTemperatureDependentSchedulerDto) {
	const Dto2ObjectTestSamples::TemperatureDependentSchedulerSampleList sampleList;

	ASSERT_THAT(sampleList, SizeIs(4));

	for (const auto& sample : sampleList) {
		const TemperatureDependentSchedulerImpl& actual = sample.getObject();
		const TemperatureDependentSchedulerDto& expected = sample.getDto();

		EXPECT_THAT(actual.toTemperatureDependentSchedulerDto(), Eq(expected));
	}
}

///////////////////////////////////////////////////////////////////////////////

enum class TemperatureDependentSchedulerDtoUpdateType {
	Nothing,
	RemainingCorrection,
	MinAdjustment,
	MaxAdjustment,
	All
};

void check(const TemperatureDependentSchedulerDtoUpdateType updateType, float remainingCorrection, unsigned minAdjustment, unsigned maxAdjustment) {

	TemperatureDependentSchedulerImpl actualTemperatureDependentScheduler(
			std::make_shared<NiceMock<MockTemperatureForecast>>(),
			std::make_shared<NiceMock<MockTemperatureHistory>>(),
			remainingCorrection,
			minAdjustment,
			maxAdjustment
	);

	float expectedRemainingCorrection = remainingCorrection;
	unsigned expectedMinAdjustment = minAdjustment;
	unsigned expectedMaxAdjustment = maxAdjustment;

	for (const auto& sample : Dto2ObjectTestSamples::TemperatureDependentSchedulerSampleList()) {
		TemperatureDependentSchedulerDto actualTemperatureDependentSchedulerDto;

		if (TemperatureDependentSchedulerDtoUpdateType::RemainingCorrection == updateType || TemperatureDependentSchedulerDtoUpdateType::All == updateType) {
			expectedRemainingCorrection = sample.getDto().getRemainingCorrection();
			actualTemperatureDependentSchedulerDto.setRemainingCorrection(expectedRemainingCorrection);
		}

		if (TemperatureDependentSchedulerDtoUpdateType::MinAdjustment == updateType || TemperatureDependentSchedulerDtoUpdateType::All == updateType) {
			expectedMinAdjustment = sample.getDto().getMinAdjustment();
			actualTemperatureDependentSchedulerDto.setMinAdjustment(expectedMinAdjustment);
		}

		if (TemperatureDependentSchedulerDtoUpdateType::MaxAdjustment == updateType || TemperatureDependentSchedulerDtoUpdateType::All == updateType) {
			expectedMaxAdjustment = sample.getDto().getMaxAdjustment();
			actualTemperatureDependentSchedulerDto.setMaxAdjustment(expectedMaxAdjustment);
		}

		actualTemperatureDependentScheduler.updateFromTemperatureDependentSchedulerDto(actualTemperatureDependentSchedulerDto);

		EXPECT_THAT(actualTemperatureDependentScheduler.getRemainingCorrection(), Eq(expectedRemainingCorrection));
		EXPECT_THAT(actualTemperatureDependentScheduler.getMinAdjustment(), Eq(expectedMinAdjustment));
		EXPECT_THAT(actualTemperatureDependentScheduler.getMaxAdjustment(), Eq(expectedMaxAdjustment));
	}
}

TEST(TemperatureDependentSchedulerImplFromDtoTest, updateFromTemperatureDependentSchedulerDto_empty) {
	for (const auto& sample : Dto2ObjectTestSamples::TemperatureDependentSchedulerSampleList()) {
		const TemperatureDependentSchedulerImpl& actual = sample.getObject();

		check(TemperatureDependentSchedulerDtoUpdateType::Nothing,
				actual.getRemainingCorrection(),
				actual.getMinAdjustment(),
				actual.getMaxAdjustment()
			);
	}
}

TEST(TemperatureDependentSchedulerImplFromDtoTest, updateFromTemperatureDependentSchedulerDto_partial_remainingCorrection) {
	for (const auto& sample : Dto2ObjectTestSamples::TemperatureDependentSchedulerSampleList()) {
		const TemperatureDependentSchedulerImpl& actual = sample.getObject();

		check(TemperatureDependentSchedulerDtoUpdateType::RemainingCorrection,
				actual.getRemainingCorrection(),
				actual.getMinAdjustment(),
				actual.getMaxAdjustment()
			);
	}
}

TEST(TemperatureDependentSchedulerImplFromDtoTest, updateFromTemperatureDependentSchedulerDto_partial_minAdjustment) {
	for (const auto& sample : Dto2ObjectTestSamples::TemperatureDependentSchedulerSampleList()) {
		const TemperatureDependentSchedulerImpl& actual = sample.getObject();

		check(TemperatureDependentSchedulerDtoUpdateType::MinAdjustment,
				actual.getRemainingCorrection(),
				actual.getMinAdjustment(),
				actual.getMaxAdjustment()
			);
	}
}

TEST(TemperatureDependentSchedulerImplFromDtoTest, updateFromTemperatureDependentSchedulerDto_partial_maxAdjustment) {
	for (const auto& sample : Dto2ObjectTestSamples::TemperatureDependentSchedulerSampleList()) {
		const TemperatureDependentSchedulerImpl& actual = sample.getObject();

		check(TemperatureDependentSchedulerDtoUpdateType::MaxAdjustment,
				actual.getRemainingCorrection(),
				actual.getMinAdjustment(),
				actual.getMaxAdjustment()
			);
	}
}

TEST(TemperatureDependentSchedulerImplFromDtoTest, updateFromTemperatureDependentSchedulerDto_all) {
	for (const auto& sample : Dto2ObjectTestSamples::TemperatureDependentSchedulerSampleList()) {
		const TemperatureDependentSchedulerImpl& actual = sample.getObject();

		check(TemperatureDependentSchedulerDtoUpdateType::All,
				actual.getRemainingCorrection(),
				actual.getMinAdjustment(),
				actual.getMaxAdjustment()
			);
	}
}
