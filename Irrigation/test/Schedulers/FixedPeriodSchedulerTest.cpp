#include "FixedPeriodSchedulerTest.h"
#include "Utils/TimeConversion.h"
#include "Logger/Logger.h"

using namespace std;
using namespace testing;


void FixedPeriodSchedulerTest::SetUp() {
//	LOGGER.setLevel(LogLevel::TRACE);
//	LOGGER.setOutputStream(cout);

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

extern time_t toLocalTime(int year, int month, int day, int hour, int min, int sec);

TEST_F(FixedPeriodSchedulerTest, getAdjustment) {
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 25)));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 4, 0, 0)), Eq(Scheduler::Result(true, true, 80)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(80));
}

TEST_F(FixedPeriodSchedulerTest, getAdjustmentLower) {
	const int forecastedTemp1 = 9;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(0));
}

TEST_F(FixedPeriodSchedulerTest, getAdjustmentHigher) {
	const int forecastedTemp1 = 35;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 4, 0, 0)), Eq(Scheduler::Result(true, true, 120)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(120));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(FixedPeriodSchedulerTest, getAdjustmentWithoutCorrection1) {
	const int forecastedTemp1 = 20;
	const int forecastedTemp2 = 12;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 29, 4, 0, 0), toLocalTime(2020, 3, 1, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp2)));

	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, forecastedTemp1, 0)));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 4, 0, 0)), Eq(Scheduler::Result(true, true, 60)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(60));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 29, 4, 0, 0)), Eq(Scheduler::Result(true, true, 28)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(28));
}

TEST_F(FixedPeriodSchedulerTest, getAdjustmentWithCorrection1) {
	const int forecastedTemp1 = 20;
	const int forecastedTemp2 = 12;
	const int measuredTemp1 = forecastedTemp1 + 5;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 29, 4, 0, 0), toLocalTime(2020, 3, 1, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp2)));

	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, measuredTemp1, 0)));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 4, 0, 0)), Eq(Scheduler::Result(true, true, 60)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(60));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 29, 4, 0, 0)), Eq(Scheduler::Result(true, true, 48)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(28));
}

TEST_F(FixedPeriodSchedulerTest, getAdjustmentSameDay1) {
	const int forecastedTemp1 = 20;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 4, 0, 0)), Eq(Scheduler::Result(true, true, 60)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(60));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 10, 0, 0)), Eq(Scheduler::Result(false, true, 0)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(60));
}

TEST_F(FixedPeriodSchedulerTest, getAdjustmentOlderDay1) {
	const int forecastedTemp1 = 20;
	const int forecastedTemp2 = 12;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 20, 4, 0, 0), toLocalTime(2020, 2, 21, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 22, 4, 0, 0), toLocalTime(2020, 2, 23, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp2)));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 20, 4, 0, 0)), Eq(Scheduler::Result(true, true, 60)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(60));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 22, 4, 0, 0)), Eq(Scheduler::Result(true, true, 28)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(28));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(FixedPeriodSchedulerTest, getAdjustmentWithoutCorrection2) {
	const int forecastedTemp1 = 15;
	const int forecastedTemp2 = 22;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 29, 4, 0, 0), toLocalTime(2020, 3, 1, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp2)));

	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, forecastedTemp1, 0)));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 4, 0, 0)), Eq(Scheduler::Result(true, true, 40)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(40));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 29, 4, 0, 0)), Eq(Scheduler::Result(true, true, 68)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(68));
}

TEST_F(FixedPeriodSchedulerTest, getAdjustmentWithCorrection2) {
	const int forecastedTemp1 = 15;
	const int forecastedTemp2 = 22;
	const int measuredTemp1 = forecastedTemp1 - 5;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 29, 4, 0, 0), toLocalTime(2020, 3, 1, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp2)));

	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, measuredTemp1, 0)));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 4, 0, 0)), Eq(Scheduler::Result(true, true, 40)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(40));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 29, 4, 0, 0)), Eq(Scheduler::Result(true, true, 48)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(68));
}

TEST_F(FixedPeriodSchedulerTest, getAdjustmentSameDay2) {
	const int forecastedTemp1 = 15;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 4, 0, 0)), Eq(Scheduler::Result(true, true, 40)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(40));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 10, 0, 0)), Eq(Scheduler::Result(false, true, 0)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(40));
}

TEST_F(FixedPeriodSchedulerTest, getAdjustmentOlderDay2) {
	const int forecastedTemp1 = 15;
	const int forecastedTemp2 = 22;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 20, 4, 0, 0), toLocalTime(2020, 2, 21, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 22, 4, 0, 0), toLocalTime(2020, 2, 23, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp2)));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 20, 4, 0, 0)), Eq(Scheduler::Result(true, true, 40)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(40));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 22, 4, 0, 0)), Eq(Scheduler::Result(true, true, 68)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(68));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(FixedPeriodSchedulerTest, dayStart) {
	const int forecastedTemp1 = 15;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 0, 30, 0), toLocalTime(2020, 2, 29, 0, 29, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 0, 30, 0)), Eq(Scheduler::Result(true, true, 40)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(40));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 23, 30, 0)), Eq(Scheduler::Result(false, true, 0)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(40));
}

