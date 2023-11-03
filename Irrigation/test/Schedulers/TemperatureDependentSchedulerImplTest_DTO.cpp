#include "TemperatureDependentSchedulerImplTest.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const float TemperatureDependentSchedulerImplDtoTest::originalRemainingCorrection = 5.678f;
const float TemperatureDependentSchedulerImplDtoTest::newRemainingCorrection = 6.789f;

///////////////////////////////////////////////////////////////////////////////

void TemperatureDependentSchedulerImplDtoTest::SetUp() {
	TemperatureDependentSchedulerImplTest::SetUp();
	temperatureDependentScheduler->setRemainingCorrection(originalRemainingCorrection);
	temperatureDependentScheduler->setMinAdjustment(originalMinAdjustment);
	temperatureDependentScheduler->setMaxAdjustment(originalMaxAdjustment);
}

void TemperatureDependentSchedulerImplDtoTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(TemperatureDependentSchedulerImplDtoTest, toTemperatureDependentSchedulerDto) {
	TemperatureDependentSchedulerDto expectedTemperatureDependentSchedulerDto(
			originalRemainingCorrection,
			originalMinAdjustment,
			originalMaxAdjustment
		);

	EXPECT_THAT(temperatureDependentScheduler->toTemperatureDependentSchedulerDto(), Eq(expectedTemperatureDependentSchedulerDto));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(TemperatureDependentSchedulerImplDtoTest, updateFromTemperatureDependentSchedulerDto_empty) {
	TemperatureDependentSchedulerDto temperatureDependentSchedulerDto;

	temperatureDependentScheduler->updateFromTemperatureDependentSchedulerDto(temperatureDependentSchedulerDto);

	EXPECT_THAT(temperatureDependentScheduler->getRemainingCorrection(), Eq(originalRemainingCorrection));
	EXPECT_THAT(temperatureDependentScheduler->getMinAdjustment(), Eq(originalMinAdjustment));
	EXPECT_THAT(temperatureDependentScheduler->getMaxAdjustment(), Eq(originalMaxAdjustment));
}

TEST_F(TemperatureDependentSchedulerImplDtoTest, updateFromTemperatureDependentSchedulerDto_remainingCorrection) {
	TemperatureDependentSchedulerDto temperatureDependentSchedulerDto;
	temperatureDependentSchedulerDto.setRemainingCorrection(newRemainingCorrection);

	temperatureDependentScheduler->updateFromTemperatureDependentSchedulerDto(temperatureDependentSchedulerDto);

	EXPECT_THAT(temperatureDependentScheduler->getRemainingCorrection(), Eq(newRemainingCorrection));
	EXPECT_THAT(temperatureDependentScheduler->getMinAdjustment(), Eq(originalMinAdjustment));
	EXPECT_THAT(temperatureDependentScheduler->getMaxAdjustment(), Eq(originalMaxAdjustment));
}

TEST_F(TemperatureDependentSchedulerImplDtoTest, updateFromTemperatureDependentSchedulerDto_minAdjustment) {
	TemperatureDependentSchedulerDto temperatureDependentSchedulerDto;
	temperatureDependentSchedulerDto.setMinAdjustment(newMinAdjustment);

	temperatureDependentScheduler->updateFromTemperatureDependentSchedulerDto(temperatureDependentSchedulerDto);

	EXPECT_THAT(temperatureDependentScheduler->getRemainingCorrection(), Eq(originalRemainingCorrection));
	EXPECT_THAT(temperatureDependentScheduler->getMinAdjustment(), Eq(newMinAdjustment));
	EXPECT_THAT(temperatureDependentScheduler->getMaxAdjustment(), Eq(originalMaxAdjustment));
}

TEST_F(TemperatureDependentSchedulerImplDtoTest, updateFromTemperatureDependentSchedulerDto_maxAdjustment) {
	TemperatureDependentSchedulerDto temperatureDependentSchedulerDto;
	temperatureDependentSchedulerDto.setMaxAdjustment(newMaxAdjustment);

	temperatureDependentScheduler->updateFromTemperatureDependentSchedulerDto(temperatureDependentSchedulerDto);

	EXPECT_THAT(temperatureDependentScheduler->getRemainingCorrection(), Eq(originalRemainingCorrection));
	EXPECT_THAT(temperatureDependentScheduler->getMinAdjustment(), Eq(originalMinAdjustment));
	EXPECT_THAT(temperatureDependentScheduler->getMaxAdjustment(), Eq(newMaxAdjustment));
}

TEST_F(TemperatureDependentSchedulerImplDtoTest, updateFromTemperatureDependentSchedulerDto_all) {
	const TemperatureDependentSchedulerDto temperatureDependentSchedulerDto(
			newRemainingCorrection,
			newMinAdjustment,
			newMaxAdjustment
		);

	temperatureDependentScheduler->updateFromTemperatureDependentSchedulerDto(temperatureDependentSchedulerDto);

	EXPECT_THAT(temperatureDependentScheduler->getRemainingCorrection(), Eq(newRemainingCorrection));
	EXPECT_THAT(temperatureDependentScheduler->getMinAdjustment(), Eq(newMinAdjustment));
	EXPECT_THAT(temperatureDependentScheduler->getMaxAdjustment(), Eq(newMaxAdjustment));
}
