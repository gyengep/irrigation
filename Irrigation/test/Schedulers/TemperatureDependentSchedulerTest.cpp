#include "TemperatureDependentSchedulerTest.h"

using namespace std;
using namespace testing;


void TemperatureDependentSchedulerTest::SetUp() {
	mockTemperatureForecast = make_shared<MockTemperatureForecast>();
	mockTemperatureHistory = make_shared<MockTemperatureHistory>();
	scheduler = unique_ptr<TemperatureDependentScheduler>(new TemperatureDependentScheduler(mockTemperatureForecast, mockTemperatureHistory));
}

void TemperatureDependentSchedulerTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(TemperatureDependentSchedulerTest, getRequiredPercentForNextDay) {
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(1000, 1000 + 24 * 60 * 60 - 1)).
		WillOnce(Return(TemperatureForecast::Values(0, 25)));

	EXPECT_THAT(scheduler->getRequiredPercentForNextDay(1000), Eq(50));
}

TEST_F(TemperatureDependentSchedulerTest, getRequiredPercentForPreviousDay) {
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(10000 - 24 * 60 * 60, 9999)).
		WillOnce(Return(TemperatureHistory::Values(0, 25, 0)));

	EXPECT_THAT(scheduler->getRequiredPercentForPreviousDay(10000), Eq(50));
}
