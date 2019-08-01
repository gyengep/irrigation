#include "FixedAmountSchedulerTest.h"
#include "Utils/TimeConversion.h"
#include "Logger/Logger.h"

using namespace std;
using namespace testing;

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

///////////////////////////////////////////////////////////////////////////////

time_t toLocalTime(int year, int month, int day, int hour, int min, int sec) {
	tm timeinfo = toCalendarTime(year, month, day, hour, min, sec);
	return timelocal(&timeinfo);
}

TEST_F(FixedAmountSchedulerTest, getAdjustment) {
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 25)));

	scheduler->process(toCalendarTime(2020, 2, 28, 4, 0, 0));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 28, 4, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));
}

TEST_F(FixedAmountSchedulerTest, getAdjustmentLower) {
	const int forecastedTemp1 = 9;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));

	scheduler->process(toCalendarTime(2020, 2, 28, 4, 0, 0));
	EXPECT_FALSE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 28, 4, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(0));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(0));
}
/*
TEST_F(FixedAmountSchedulerTest, getAdjustmentHigher) {
	const int forecastedTemp1 = 35;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));

	scheduler->process(toCalendarTime(2020, 2, 28, 4, 0, 0));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 28, 4, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(120));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(120));
}
*/

///////////////////////////////////////////////////////////////////////////////

TEST_F(FixedAmountSchedulerTest, getAdjustmentWithoutCorrection1) {
	const int forecastedTemp1 = 20;
	const int forecastedTemp2 = 12;
	const int measuredTemp1 = forecastedTemp1;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 29, 4, 0, 0), toLocalTime(2020, 3, 1, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp2)));

	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, measuredTemp1, 0)));

	scheduler->process(toCalendarTime(2020, 2, 28, 4, 0, 0));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 28, 4, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	scheduler->process(toCalendarTime(2020, 2, 29, 4, 0, 0));
	EXPECT_FALSE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 29, 4, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(0));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(40));
}

TEST_F(FixedAmountSchedulerTest, getAdjustmentWithCorrection1) {
	const int forecastedTemp1 = 20;
	const int forecastedTemp2 = 12;
	const int measuredTemp1 = forecastedTemp1 + 5;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 29, 4, 0, 0), toLocalTime(2020, 3, 1, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp2)));

	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, measuredTemp1, 0)));

	scheduler->process(toCalendarTime(2020, 2, 28, 4, 0, 0));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 28, 4, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	scheduler->process(toCalendarTime(2020, 2, 29, 4, 0, 0));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 29, 4, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(120));
}

TEST_F(FixedAmountSchedulerTest, getAdjustmentSameDay1) {
	const int forecastedTemp1 = 20;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));

	scheduler->process(toCalendarTime(2020, 2, 28, 4, 0, 0));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 28, 4, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	scheduler->process(toCalendarTime(2020, 2, 28, 10, 0, 0));
	EXPECT_FALSE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 28, 10, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(0));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));
}

TEST_F(FixedAmountSchedulerTest, getAdjustmentEarlierDay1) {
	const int forecastedTemp1 = 20;
	const int forecastedTemp2 = 12;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 20, 4, 0, 0), toLocalTime(2020, 2, 21, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 22, 4, 0, 0), toLocalTime(2020, 2, 23, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp2)));

	scheduler->process(toCalendarTime(2020, 2, 20, 4, 0, 0));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 20, 4, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	scheduler->process(toCalendarTime(2020, 2, 22, 4, 0, 0));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 22, 4, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(FixedAmountSchedulerTest, getAdjustmentWithoutCorrection2) {
	const int forecastedTemp1 = 15;
	const int forecastedTemp2 = 22;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 29, 4, 0, 0), toLocalTime(2020, 3, 1, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp2)));

	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, forecastedTemp1, 0)));

	scheduler->process(toCalendarTime(2020, 2, 28, 4, 0, 0));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 28, 4, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	scheduler->process(toCalendarTime(2020, 2, 29, 4, 0, 0));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 29, 4, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(160));
}

TEST_F(FixedAmountSchedulerTest, getAdjustmentWithCorrection2) {
	const int forecastedTemp1 = 15;
	const int forecastedTemp2 = 22;
	const int measuredTemp1 = forecastedTemp1 - 5;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 29, 4, 0, 0), toLocalTime(2020, 3, 1, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp2)));

	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, measuredTemp1, 0)));

	scheduler->process(toCalendarTime(2020, 2, 28, 4, 0, 0));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 28, 4, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	scheduler->process(toCalendarTime(2020, 2, 29, 4, 0, 0));
	EXPECT_FALSE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 29, 4, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(0));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(80));
}

TEST_F(FixedAmountSchedulerTest, getAdjustmentSameDay2) {
	const int forecastedTemp1 = 15;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));

	scheduler->process(toCalendarTime(2020, 2, 28, 4, 0, 0));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 28, 4, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	scheduler->process(toCalendarTime(2020, 2, 28, 10, 0, 0));
	EXPECT_FALSE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 28, 10, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(0));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));
}

TEST_F(FixedAmountSchedulerTest, getAdjustmentEarlierDay2) {
	const int forecastedTemp1 = 15;
	const int forecastedTemp2 = 22;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 20, 4, 0, 0), toLocalTime(2020, 2, 21, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 22, 4, 0, 0), toLocalTime(2020, 2, 23, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp2)));

	scheduler->process(toCalendarTime(2020, 2, 20, 4, 0, 0));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 20, 4, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	scheduler->process(toCalendarTime(2020, 2, 22, 4, 0, 0));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 22, 4, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(FixedAmountSchedulerTest, dayStart) {
	LOGGER.setLevel(LogLevel::TRACE);
	LOGGER.setOutputStream(cout);

	const int forecastedTemp1 = 15;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 0, 30, 0), toLocalTime(2020, 2, 29, 0, 29, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));

	scheduler->process(toCalendarTime(2020, 2, 28, 0, 30, 0));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 28, 0, 30, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	scheduler->process(toCalendarTime(2020, 2, 28, 23, 30, 0));
	EXPECT_FALSE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 28, 23, 30, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(0));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));
}
