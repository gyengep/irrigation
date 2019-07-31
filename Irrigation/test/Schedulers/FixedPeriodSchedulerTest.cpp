#include "FixedPeriodSchedulerTest.h"
#include "Utils/TimeConversion.h"

using namespace std;
using namespace testing;


void FixedPeriodSchedulerTest::SetUp() {
	mockTemperatureForecast = make_shared<MockTemperatureForecast>();
	mockTemperatureHistory = make_shared<MockTemperatureHistory>();
	scheduler.reset(new FixedPeriodScheduler(mockTemperatureForecast, mockTemperatureHistory));

	scheduler->setTemperatureAndPercents(vector<pair<float, int>>{
		{ 10.0f, 20 },
		{ 15.0f, 40 },
		{ 20.0f, 60 },
		{ 25.0f, 80 },
		{ 30.0f, 100 }
	});
}

void FixedPeriodSchedulerTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

#include "Logger/Logger.h"


TEST_F(FixedPeriodSchedulerTest, getAdjustmentWithoutCorrection) {
	LOGGER.setOutputStream(cout);
	LOGGER.setLevel(LogLevel::TRACE);

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(_, _)).
		WillOnce(Return(TemperatureForecast::Values(0, 20))).
		WillOnce(Return(TemperatureForecast::Values(0, 25))).
		WillOnce(Return(TemperatureForecast::Values(0, 30)));

	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(_, _)).
		WillOnce(Return(TemperatureHistory::Values(0, 20, 0))).
		WillOnce(Return(TemperatureHistory::Values(0, 25, 0)));

	scheduler->process(toCalendarTime(2016, 11, 20));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2016, 11, 20)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(60));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(60));

	scheduler->process(toCalendarTime(2016, 11, 21));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2016, 11, 21)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(80));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(80));

	scheduler->process(toCalendarTime(2016, 11, 22));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2016, 11, 22)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));
}

TEST_F(FixedPeriodSchedulerTest, getAdjustmentWithCorrection) {
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(_, _)).
		WillOnce(Return(TemperatureForecast::Values(0, 20))).
		WillOnce(Return(TemperatureForecast::Values(0, 25))).
		WillOnce(Return(TemperatureForecast::Values(0, 30)));

	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(_, _)).
		WillOnce(Return(TemperatureHistory::Values(0, 22, 0))).
		WillOnce(Return(TemperatureHistory::Values(0, 23, 0)));

	scheduler->process(toCalendarTime(2016, 12, 31));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2016, 12, 31)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(60));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(60));

	scheduler->process(toCalendarTime(2017, 1, 1));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2017, 1, 1)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(88));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(80));

	scheduler->process(toCalendarTime(2017, 1, 2));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2017, 1, 2)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(92));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));
}

TEST_F(FixedPeriodSchedulerTest, getAdjustmentWithNegativeCorrection) {
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(_, _)).
		WillOnce(Return(TemperatureForecast::Values(0, 30))).
		WillOnce(Return(TemperatureForecast::Values(0, 10)));

	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(_, _)).
		WillOnce(Return(TemperatureHistory::Values(0, 20, 0)));

	scheduler->process(toCalendarTime(2016, 1, 1));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2016, 1, 1)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	scheduler->process(toCalendarTime(2016, 1, 2));
	EXPECT_FALSE(scheduler->isDayScheduled(toCalendarTime(2016, 1, 2)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(0));  // -20
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(40));
}
