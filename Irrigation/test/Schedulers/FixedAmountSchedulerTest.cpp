#include "FixedAmountSchedulerTest.h"
#include "Utils/TimeConversion.h"

using namespace std;
using namespace testing;
using namespace TemperatureDependentScheduler;

void FixedAmountSchedulerTest::SetUp() {
	mockTemperatureForecast = make_shared<MockTemperatureForecast>();
	mockTemperatureHistory = make_shared<MockTemperatureHistory>();
	scheduler.reset(new FixedAmountScheduler(mockTemperatureForecast, mockTemperatureHistory));

	scheduler->setTemperatureAndPercents(vector<pair<float, int>>{
		{ 10.0f, 20 },
		{ 15.0f, 40 },
		{ 20.0f, 60 },
		{ 25.0f, 80 },
		{ 30.0f, 100 }
	});
}

void FixedAmountSchedulerTest::TearDown() {
}
/*
///////////////////////////////////////////////////////////////////////////////
#include "Logger/Logger.h"
TEST_F(FixedAmountSchedulerTest, getAdjustmentWithoutCorrection) {
	LOGGER.setLevel(LogLevel::TRACE);
	LOGGER.setOutputStream(cout);

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(_, _)).
		WillOnce(Return(TemperatureForecast::Values(0, 10))).
		WillOnce(Return(TemperatureForecast::Values(0, 15))).
		WillOnce(Return(TemperatureForecast::Values(0, 20)));

	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(_, _)).
		WillOnce(Return(TemperatureHistory::Values(0, 10, 0))).
		WillOnce(Return(TemperatureHistory::Values(0, 15, 0)));

	scheduler->process(toCalendarTime(2016, 11, 20));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2016, 11, 20)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(80)); // 100 - 20

	scheduler->process(toCalendarTime(2016, 11, 20));
	EXPECT_FALSE(scheduler->isDayScheduled(toCalendarTime(2016, 11, 20)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(0));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(40)); // 80 - 40

	scheduler->process(toCalendarTime(2016, 11, 20));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2016, 11, 20)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(80)); // 40 + 100 - 60
}

TEST_F(FixedAmountSchedulerTest, getAdjustmentWithCorrection) {
	LOGGER.setLevel(LogLevel::TRACE);
	LOGGER.setOutputStream(cout);

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(_, _)).
		WillOnce(Return(TemperatureForecast::Values(0, 10))).
		WillOnce(Return(TemperatureForecast::Values(0, 15))).
		WillOnce(Return(TemperatureForecast::Values(0, 20)));

	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(_, _)).
		WillOnce(Return(TemperatureHistory::Values(0, 13, 0))).
		WillOnce(Return(TemperatureHistory::Values(0, 12, 0)));

	scheduler->process(toCalendarTime(2016, 11, 20));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2016, 11, 20)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(80)); // 100 - 20

	scheduler->process(toCalendarTime(2016, 11, 20));
	EXPECT_FALSE(scheduler->isDayScheduled(toCalendarTime(2016, 11, 20)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(0));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(28)); // 80 - 40 - 12

	scheduler->process(toCalendarTime(2016, 11, 20));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2016, 11, 20)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(80)); // 28 + 100 - 60 + 12
}

TEST_F(FixedAmountSchedulerTest, getAdjustmentWithNegativeCorrection) {
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(_, _)).
		WillOnce(Return(TemperatureForecast::Values(0, 25))).
		WillOnce(Return(TemperatureForecast::Values(0, 10)));

	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(_, _)).
		WillOnce(Return(TemperatureHistory::Values(0, 30, 0)));

	scheduler->process(toCalendarTime(2016, 11, 20));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2016, 11, 20)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(20)); // 100 - 80

	scheduler->process(toCalendarTime(2016, 11, 20));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2016, 11, 20)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(80)); // 20 - 20 + 100 - 20
}
*/
