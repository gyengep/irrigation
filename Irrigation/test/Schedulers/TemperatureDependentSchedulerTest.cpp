#include "TemperatureDependentSchedulerTest.h"
#include "Schedulers/TemperatureToPercent.h"
#include "Utils/TimeConversion.h"
#include "Logger/Logger.h"

using namespace std;
using namespace testing;


void TemperatureDependentSchedulerTest::SetUp() {
	mockTemperatureForecast = make_shared<MockTemperatureForecast>();
	mockTemperatureHistory = make_shared<MockTemperatureHistory>();
	scheduler.reset(new TemperatureDependentScheduler(mockTemperatureForecast, mockTemperatureHistory));

	TemperatureToPercent::getInstance().setTemperatureAndPercents(vector<pair<float, int>>{
		{ 15.0f, 25 },
		{ 25.0f, 50 },
		{ 35.0f, 100 }
	});
}

void TemperatureDependentSchedulerTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

extern time_t toLocalTime(int year, int month, int day, int hour, int min, int sec);

TEST_F(TemperatureDependentSchedulerTest, getAdjustment) {
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 25)));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 50)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(50));
}

TEST_F(TemperatureDependentSchedulerTest, getAdjustmentLower) {
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 9)));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(0));
}

TEST_F(TemperatureDependentSchedulerTest, getAdjustmentHigher) {
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 36)));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 105)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(105));
}

TEST_F(TemperatureDependentSchedulerTest, getRequiredPercentForNextDay) {
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 25)));

	EXPECT_THAT(scheduler->getRequiredPercentForNextDay(toLocalTime(2019, 8, 1, 4, 0, 0)), Eq(50));
}

TEST_F(TemperatureDependentSchedulerTest, getRequiredPercentForNextDayWithCorrection1) {
	scheduler->setForecastCorrection(1.1f, 0.0f);

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 30)));

	EXPECT_THAT(scheduler->getRequiredPercentForNextDay(toLocalTime(2019, 8, 1, 4, 0, 0)), Eq(90));
}

TEST_F(TemperatureDependentSchedulerTest, getRequiredPercentForNextDayWithCorrection2) {
	scheduler->setForecastCorrection(1.1f, -1.0f);

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 30)));

	EXPECT_THAT(scheduler->getRequiredPercentForNextDay(toLocalTime(2019, 8, 1, 4, 0, 0)), Eq(85));
}

TEST_F(TemperatureDependentSchedulerTest, getRequiredPercentForPreviousDay) {
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 25, 0)));

	EXPECT_THAT(scheduler->getRequiredPercentForPreviousDay(toLocalTime(2019, 8, 2, 4, 0, 0)), Eq(50));
}

TEST_F(TemperatureDependentSchedulerTest, getRequiredPercentForPreviousDayWithCorrection1) {
	scheduler->setHistoryCorrection(1.0f, 2.0f);

	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 25, 0)));

	EXPECT_THAT(scheduler->getRequiredPercentForPreviousDay(toLocalTime(2019, 8, 2, 4, 0, 0)), Eq(60));
}

TEST_F(TemperatureDependentSchedulerTest, getRequiredPercentForPreviousDayWithCorrection2) {
	scheduler->setHistoryCorrection(0.8f, -1.0f);

	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 25, 0)));

	EXPECT_THAT(scheduler->getRequiredPercentForPreviousDay(toLocalTime(2019, 8, 2, 4, 0, 0)), Eq(35));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(TemperatureDependentSchedulerTest, getAdjustmentToday) {
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 15, 4, 0, 0), toLocalTime(2020, 2, 16, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 25)));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 15, 4, 0, 0)), Eq(Scheduler::Result(true, true, 50)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(50));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 15, 14, 0, 0)), Eq(Scheduler::Result(false, true, 0)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(50));
}

TEST_F(TemperatureDependentSchedulerTest, getAdjustmentYesterdayDay) {
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 15, 4, 0, 0), toLocalTime(2020, 2, 16, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 25)));
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2020, 2, 15, 4, 0, 0), toLocalTime(2020, 2, 16, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 25, 0)));
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 16, 4, 0, 0), toLocalTime(2020, 2, 17, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 30)));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 15, 4, 0, 0)), Eq(Scheduler::Result(true, true, 50)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(50));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 16, 4, 0, 0)), Eq(Scheduler::Result(true, true, 75)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(75));
}

TEST_F(TemperatureDependentSchedulerTest, getAdjustmentOtherDay) {
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 15, 4, 0, 0), toLocalTime(2020, 2, 16, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 25)));
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 17, 4, 0, 0), toLocalTime(2020, 2, 18, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 30)));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 15, 4, 0, 0)), Eq(Scheduler::Result(true, true, 50)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(50));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 17, 4, 0, 0)), Eq(Scheduler::Result(true, true, 75)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(75));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(TemperatureDependentSchedulerTest, dayStart) {
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 0, 0, 0), toLocalTime(2020, 2, 28, 23, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 15)));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 0, 0, 0)), Eq(Scheduler::Result(true, true, 25)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(25));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 23, 59, 59)), Eq(Scheduler::Result(false, true, 0)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(25));
}
