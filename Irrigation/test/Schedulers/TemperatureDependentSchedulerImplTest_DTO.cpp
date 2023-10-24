#include "TemperatureDependentSchedulerImplTest.h"
#include "Dto2ObjectSamples/TemperatureDependentSchedulerSamples.h"

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

void TemperatureDependentSchedulerImplFromDtoTest::checkUpdateFromTemperatureDependentSchedulerDto(const UpdateType updateType) {
	const auto sampleList = Dto2ObjectTestSamples::TemperatureDependentSchedulerSampleList();

	float expectedRemainingCorrection = temperatureDependentScheduler->getRemainingCorrection();
	unsigned expectedMinAdjustment = temperatureDependentScheduler->getMinAdjustment();
	unsigned expectedMaxAdjustment = temperatureDependentScheduler->getMaxAdjustment();

	for (const auto& sample : sampleList) {
		TemperatureDependentSchedulerDto actualTemperatureDependentSchedulerDto;

		if (UpdateType::RemainingCorrection == updateType || UpdateType::All == updateType) {
			expectedRemainingCorrection = sample.getDto().getRemainingCorrection();
			actualTemperatureDependentSchedulerDto.setRemainingCorrection(expectedRemainingCorrection);
		}

		if (UpdateType::MinAdjustment == updateType || UpdateType::All == updateType) {
			expectedMinAdjustment = sample.getDto().getMinAdjustment();
			actualTemperatureDependentSchedulerDto.setMinAdjustment(expectedMinAdjustment);
		}

		if (UpdateType::MaxAdjustment == updateType || UpdateType::All == updateType) {
			expectedMaxAdjustment = sample.getDto().getMaxAdjustment();
			actualTemperatureDependentSchedulerDto.setMaxAdjustment(expectedMaxAdjustment);
		}

		temperatureDependentScheduler->updateFromTemperatureDependentSchedulerDto(actualTemperatureDependentSchedulerDto);

		EXPECT_THAT(temperatureDependentScheduler->getRemainingCorrection(), Eq(expectedRemainingCorrection));
		EXPECT_THAT(temperatureDependentScheduler->getMinAdjustment(), Eq(expectedMinAdjustment));
		EXPECT_THAT(temperatureDependentScheduler->getMaxAdjustment(), Eq(expectedMaxAdjustment));
	}
}

TEST_F(TemperatureDependentSchedulerImplFromDtoTest, updateFromTemperatureDependentSchedulerDto_empty) {
	checkUpdateFromTemperatureDependentSchedulerDto(UpdateType::Nothing);
}

TEST_F(TemperatureDependentSchedulerImplFromDtoTest, updateFromTemperatureDependentSchedulerDto_partial_remainingCorrection) {
	checkUpdateFromTemperatureDependentSchedulerDto(UpdateType::RemainingCorrection);
}

TEST_F(TemperatureDependentSchedulerImplFromDtoTest, updateFromTemperatureDependentSchedulerDto_partial_minAdjustment) {
	checkUpdateFromTemperatureDependentSchedulerDto(UpdateType::MinAdjustment);
}

TEST_F(TemperatureDependentSchedulerImplFromDtoTest, updateFromTemperatureDependentSchedulerDto_partial_maxAdjustment) {
	checkUpdateFromTemperatureDependentSchedulerDto(UpdateType::MaxAdjustment);
}

TEST_F(TemperatureDependentSchedulerImplFromDtoTest, updateFromTemperatureDependentSchedulerDto_all) {
	checkUpdateFromTemperatureDependentSchedulerDto(UpdateType::All);
}
