#include "Schedulers/HotWeatherSchedulerImpl.h"
#include "Mocks/MockTemperatureHistory.h"
#include <gmock/gmock.h>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

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
