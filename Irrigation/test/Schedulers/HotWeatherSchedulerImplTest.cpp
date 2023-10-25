#include "HotWeatherSchedulerImplTest.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(HotWeatherSchedulerImplConstructorTest, defaultConstructor) {
	HotWeatherSchedulerImpl scheduler(
		std::make_shared<StrictMock<MockTemperatureHistory>>()
	);

	EXPECT_THAT(scheduler.getMinTemperature(), Eq(35));
	EXPECT_THAT(scheduler.getPeriod(), Eq(std::chrono::hours(2)));
}

TEST(HotWeatherSchedulerImplConstructorTest, parametrizedConstructor) {
	HotWeatherSchedulerImpl scheduler(
		std::make_shared<StrictMock<MockTemperatureHistory>>(),
		std::chrono::minutes(3),
		15
	);

	EXPECT_THAT(scheduler.getPeriod(), Eq(std::chrono::minutes(3)));
	EXPECT_THAT(scheduler.getMinTemperature(), Eq(15));
}

///////////////////////////////////////////////////////////////////////////////

TEST(HotWeatherSchedulerImplFactoryTest, create) {
	auto mockTemperatureHistory = std::make_shared<NiceMock<MockTemperatureHistory>>();

	HotWeatherSchedulerImplFactory hotWeatherSchedulerImplFactory(
			mockTemperatureHistory
		);

	EXPECT_THAT(hotWeatherSchedulerImplFactory.create(), Not(IsNull()));
	EXPECT_TRUE(hotWeatherSchedulerImplFactory.create() != hotWeatherSchedulerImplFactory.create());
}

///////////////////////////////////////////////////////////////////////////////

void HotWeatherSchedulerImplTest::SetUp() {
	mockTemperatureHistory = std::make_shared<StrictMock<MockTemperatureHistory>>();
	hotWeatherScheduler = std::make_shared<HotWeatherSchedulerImpl>(mockTemperatureHistory);
}

void HotWeatherSchedulerImplTest::TearDown() {

}
