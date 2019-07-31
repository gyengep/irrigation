#include "TemperatureDependentSchedulerTest.h"

using namespace std;
using namespace testing;


void TemperatureDependentSchedulerTest::SetUp() {
	mockTemperatureForecast = make_shared<MockTemperatureForecast>();
	mockTemperatureHistory = make_shared<MockTemperatureHistory>();
	scheduler = unique_ptr<TemperatureDependentScheduler>(new TemperatureDependentScheduler(mockTemperatureForecast, mockTemperatureHistory));

	scheduler->setTemperatureAndPercents(vector<pair<float, int>>{
		{ 15.0f, 25 },
		{ 25.0f, 50 },
		{ 35.0f, 100 }
	});
}

void TemperatureDependentSchedulerTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(TemperatureDependentSchedulerTest, getRequiredPercentFromTemperature) {
	EXPECT_THAT(scheduler->getRequiredPercentFromTemperature(15.0f), Eq(25));
	EXPECT_THAT(scheduler->getRequiredPercentFromTemperature(16.0f), Eq(27));
	EXPECT_THAT(scheduler->getRequiredPercentFromTemperature(17.0f), Eq(30));
	EXPECT_THAT(scheduler->getRequiredPercentFromTemperature(18.0f), Eq(32));
	EXPECT_THAT(scheduler->getRequiredPercentFromTemperature(19.0f), Eq(35));
	EXPECT_THAT(scheduler->getRequiredPercentFromTemperature(20.0f), Eq(37));
	EXPECT_THAT(scheduler->getRequiredPercentFromTemperature(21.0f), Eq(40));
	EXPECT_THAT(scheduler->getRequiredPercentFromTemperature(22.0f), Eq(42));
	EXPECT_THAT(scheduler->getRequiredPercentFromTemperature(23.0f), Eq(45));
	EXPECT_THAT(scheduler->getRequiredPercentFromTemperature(24.0f), Eq(47));
	EXPECT_THAT(scheduler->getRequiredPercentFromTemperature(25.0f), Eq(50));
	EXPECT_THAT(scheduler->getRequiredPercentFromTemperature(26.0f), Eq(55));
	EXPECT_THAT(scheduler->getRequiredPercentFromTemperature(27.0f), Eq(60));
	EXPECT_THAT(scheduler->getRequiredPercentFromTemperature(28.0f), Eq(65));
	EXPECT_THAT(scheduler->getRequiredPercentFromTemperature(29.0f), Eq(70));
	EXPECT_THAT(scheduler->getRequiredPercentFromTemperature(30.0f), Eq(75));
	EXPECT_THAT(scheduler->getRequiredPercentFromTemperature(31.0f), Eq(80));
	EXPECT_THAT(scheduler->getRequiredPercentFromTemperature(32.0f), Eq(85));
	EXPECT_THAT(scheduler->getRequiredPercentFromTemperature(33.0f), Eq(90));
	EXPECT_THAT(scheduler->getRequiredPercentFromTemperature(34.0f), Eq(95));
	EXPECT_THAT(scheduler->getRequiredPercentFromTemperature(35.0f), Eq(100));
}

TEST_F(TemperatureDependentSchedulerTest, getRequiredPercentForNextDay) {
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(1000, 1000 + 24 * 60 * 60)).
		WillOnce(Return(TemperatureForecast::Values(0, 25)));

	EXPECT_THAT(scheduler->getRequiredPercentForNextDay(1000), Eq(50));
}

TEST_F(TemperatureDependentSchedulerTest, getRequiredPercentForPreviousDay) {
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(10000 - 24 * 60 * 60, 10000)).
		WillOnce(Return(TemperatureHistory::Values(0, 25, 0)));

	EXPECT_THAT(scheduler->getRequiredPercentForPreviousDay(10000), Eq(50));
}
