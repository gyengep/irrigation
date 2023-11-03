#include "HotWeatherSchedulerImplTest.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const float HotWeatherSchedulerImplDtoTest::originalMinTemperature = 12.3f;
const float HotWeatherSchedulerImplDtoTest::newMinTemperature = 23.4f;

///////////////////////////////////////////////////////////////////////////////

void HotWeatherSchedulerImplDtoTest::SetUp() {
	HotWeatherSchedulerImplTest::SetUp();
	hotWeatherScheduler->setPeriod(std::chrono::seconds(originalPeriodInSeconds));
	hotWeatherScheduler->setMinTemperature(originalMinTemperature);
}

void HotWeatherSchedulerImplDtoTest::TearDown() {

}

///////////////////////////////////////////////////////////////////////////////

TEST_F(HotWeatherSchedulerImplDtoTest, toHotWeatherSchedulerDto) {
	const HotWeatherSchedulerDto expectedHotWeatherSchedulerDto(
			originalPeriodInSeconds,
			originalMinTemperature
		);

	EXPECT_THAT(hotWeatherScheduler->toHotWeatherSchedulerDto(), Eq(expectedHotWeatherSchedulerDto));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(HotWeatherSchedulerImplDtoTest, updateFromHotWeatherSchedulerDto_empty) {
	HotWeatherSchedulerDto hotWeatherSchedulerDto;

	hotWeatherScheduler->updateFromHotWeatherSchedulerDto(hotWeatherSchedulerDto);

	EXPECT_THAT(hotWeatherScheduler->getPeriod(), Eq(std::chrono::seconds(originalPeriodInSeconds)));
	EXPECT_THAT(hotWeatherScheduler->getMinTemperature(), Eq(originalMinTemperature));
}

TEST_F(HotWeatherSchedulerImplDtoTest, updateFromHotWeatherSchedulerDto_period) {
	HotWeatherSchedulerDto hotWeatherSchedulerDto;
	hotWeatherSchedulerDto.setPeriodInSeconds(newPeriodInSeconds);

	hotWeatherScheduler->updateFromHotWeatherSchedulerDto(hotWeatherSchedulerDto);

	EXPECT_THAT(hotWeatherScheduler->getPeriod(), Eq(std::chrono::seconds(newPeriodInSeconds)));
	EXPECT_THAT(hotWeatherScheduler->getMinTemperature(), Eq(originalMinTemperature));
}

TEST_F(HotWeatherSchedulerImplDtoTest, updateFromHotWeatherSchedulerDto_minTemperature) {
	HotWeatherSchedulerDto hotWeatherSchedulerDto;
	hotWeatherSchedulerDto.setMinTemperature(newMinTemperature);

	hotWeatherScheduler->updateFromHotWeatherSchedulerDto(hotWeatherSchedulerDto);

	EXPECT_THAT(hotWeatherScheduler->getPeriod(), Eq(std::chrono::seconds(originalPeriodInSeconds)));
	EXPECT_THAT(hotWeatherScheduler->getMinTemperature(), Eq(newMinTemperature));
}

TEST_F(HotWeatherSchedulerImplDtoTest, updateFromHotWeatherSchedulerDto_all) {
	const HotWeatherSchedulerDto hotWeatherSchedulerDto(
			newPeriodInSeconds,
			newMinTemperature
		);

	hotWeatherScheduler->updateFromHotWeatherSchedulerDto(hotWeatherSchedulerDto);

	EXPECT_THAT(hotWeatherScheduler->getPeriod(), Eq(std::chrono::seconds(newPeriodInSeconds)));
	EXPECT_THAT(hotWeatherScheduler->getMinTemperature(), Eq(newMinTemperature));
}
