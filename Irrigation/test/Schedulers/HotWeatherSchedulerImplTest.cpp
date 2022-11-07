#include "Mocks/MockTemperatureHistory.h"
#include "Schedulers/HotWeatherSchedulerImpl.h"
#include "Dto2Object/HotWeatherSchedulerSamples.h"
#include <gmock/gmock.h>

using namespace testing;


TEST(HotWeatherSchedulerImplTest, defaultConstructor) {
	HotWeatherSchedulerImpl scheduler(
		std::make_shared<MockTemperatureHistory>()
	);

	EXPECT_THAT(scheduler.getMinTemperature(), Eq(35));
	EXPECT_THAT(scheduler.getPeriod(), Eq(2 * 60 * 60));
}

TEST(HotWeatherSchedulerImplTest, parametrizedConstructor) {
	HotWeatherSchedulerImpl scheduler(
		std::make_shared<MockTemperatureHistory>(),
		std::chrono::minutes(3),
		15
	);

	EXPECT_THAT(scheduler.getPeriod(), Eq(180));
	EXPECT_THAT(scheduler.getMinTemperature(), Eq(15));
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEST(HotWeatherSchedulerImplTest, toHotWeatherSchedulerDto) {
	EXPECT_THAT(
		Dto2ObjectTest::HotWeatherSchedulerSample1().getObjectPtr()->toHotWeatherSchedulerDto(),
		Eq(Dto2ObjectTest::HotWeatherSchedulerSample1().getDto())
	);

	EXPECT_THAT(
		Dto2ObjectTest::HotWeatherSchedulerSample2().getObjectPtr()->toHotWeatherSchedulerDto(),
		Eq(Dto2ObjectTest::HotWeatherSchedulerSample2().getDto())
	);

	EXPECT_THAT(
		Dto2ObjectTest::HotWeatherSchedulerSample3().getObjectPtr()->toHotWeatherSchedulerDto(),
		Eq(Dto2ObjectTest::HotWeatherSchedulerSample3().getDto())
	);

	EXPECT_THAT(
		Dto2ObjectTest::HotWeatherSchedulerSample4().getObjectPtr()->toHotWeatherSchedulerDto(),
		Eq(Dto2ObjectTest::HotWeatherSchedulerSample4().getDto())
	);
}

///////////////////////////////////////////////////////////////////////////////

TEST(HotWeatherSchedulerImplTest, updateFromDto_empty) {
	const unsigned expectedPeriod = 99;
	const float expectedMinTemperature = 68.0f;

	HotWeatherSchedulerImpl scheduler(
		std::make_shared<MockTemperatureHistory>(),
		std::chrono::seconds(expectedPeriod),
		expectedMinTemperature
	);

	scheduler.updateFromHotWeatherSchedulerDto(
		HotWeatherSchedulerDTO()
	);

	EXPECT_THAT(scheduler.getPeriod(), Eq(expectedPeriod));
	EXPECT_THAT(scheduler.getMinTemperature(), FloatEq(expectedMinTemperature));
}

TEST(HotWeatherSchedulerImplTest, updateFromDto_partial_period) {
	const unsigned expectedPeriod = 99;
	const float expectedMinTemperature = 68;

	HotWeatherSchedulerImpl scheduler(
		std::make_shared<MockTemperatureHistory>(),
		std::chrono::seconds(80),
		expectedMinTemperature
	);

	EXPECT_THAT(scheduler.getPeriod(), Not(Eq(expectedPeriod)));
	EXPECT_THAT(scheduler.getMinTemperature(), FloatEq(expectedMinTemperature));

	scheduler.updateFromHotWeatherSchedulerDto(
		HotWeatherSchedulerDTO().
			setPeriodInSeconds(expectedPeriod)
	);

	EXPECT_THAT(scheduler.getPeriod(), Eq(expectedPeriod));
	EXPECT_THAT(scheduler.getMinTemperature(), FloatEq(expectedMinTemperature));
}

TEST(HotWeatherSchedulerImplTest, updateFromDto_partial_minTemperature) {
	const unsigned expectedPeriod = 99;
	const float expectedMinTemperature = 68.0f;

	HotWeatherSchedulerImpl scheduler(
		std::make_shared<MockTemperatureHistory>(),
		std::chrono::seconds(expectedPeriod),
		70.0f
	);

	EXPECT_THAT(scheduler.getPeriod(), Eq(expectedPeriod));
	EXPECT_THAT(scheduler.getMinTemperature(), Not(FloatEq(expectedMinTemperature)));

	scheduler.updateFromHotWeatherSchedulerDto(
		HotWeatherSchedulerDTO().
			setMinTemperature(expectedMinTemperature)
	);

	EXPECT_THAT(scheduler.getPeriod(), Eq(expectedPeriod));
	EXPECT_THAT(scheduler.getMinTemperature(), FloatEq(expectedMinTemperature));
}

TEST(HotWeatherSchedulerImplTest, updateFromDto_all) {
	const unsigned expectedPeriod = 99;
	const float expectedMinTemperature = 68.0f;

	HotWeatherSchedulerImpl scheduler(
			std::make_shared<MockTemperatureHistory>(),
			std::chrono::seconds(80),
			15.0f
		);

	EXPECT_THAT(scheduler.getPeriod(), Not(Eq(expectedPeriod)));
	EXPECT_THAT(scheduler.getMinTemperature(), Not(FloatEq(expectedMinTemperature)));

	scheduler.updateFromHotWeatherSchedulerDto(
		HotWeatherSchedulerDTO().
			setPeriodInSeconds(expectedPeriod).
			setMinTemperature(expectedMinTemperature)
	);

	EXPECT_THAT(scheduler.getPeriod(), Eq(expectedPeriod));
	EXPECT_THAT(scheduler.getMinTemperature(), FloatEq(expectedMinTemperature));
}
