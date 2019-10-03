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

	scheduler->setRemainingCorrection(1.0f);
	scheduler->setForecastCorrection(1.0f, 0.0f);
	scheduler->setHistoryCorrection(1.0f, 0.0f);
	scheduler->setMinAdjustment(0);
	scheduler->setMaxAdjustment(0);
	scheduler->trimAdjustmentOver(0);

	TemperatureToPercent::getInstance().setTemperatureAndPercents(vector<pair<float, int>>{
		{ 15.0f, 25 },
		{ 25.0f, 50 },
		{ 35.0f, 100 }
	});
}

void TemperatureDependentSchedulerTest::TearDown() {
	LOGGER.setLevel(LogLevel::OFF);
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

///////////////////////////////////////////////////////////////////////////////

TEST_F(TemperatureDependentSchedulerTest, trim1) {
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 29.0f)));
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 33.0f, 0)));

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 2, 4, 0, 0), toLocalTime(2019, 8, 3, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 29.0f)));

	scheduler->trimAdjustmentOver(100);

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 70)));	// 70 : 90
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(70));

	// remaining: 70 - 90 = -20
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 90)));	// 70
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(70));
}

TEST_F(TemperatureDependentSchedulerTest, trim2) {
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 29.0f)));
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 33.0f, 0)));

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 2, 4, 0, 0), toLocalTime(2019, 8, 3, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 33.0f)));

	scheduler->trimAdjustmentOver(100);

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 70)));	// 70 : 90
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(70));

	// remaining: 70 - 90 = -20
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 110)));	// 90
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(90));
}

TEST_F(TemperatureDependentSchedulerTest, trim3) {
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 29.0f)));
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 33.0f, 0)));

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 2, 4, 0, 0), toLocalTime(2019, 8, 3, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 37.0f)));

	scheduler->trimAdjustmentOver(100);

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 70)));	// 70 : 90
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(70));

	// remaining: 70 - 90 = -20
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 120)));	// 110
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));
}

TEST_F(TemperatureDependentSchedulerTest, trim4) {
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 33.0f)));
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 29.0f, 0)));

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 2, 4, 0, 0), toLocalTime(2019, 8, 3, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 33.0f)));

	
	scheduler->trimAdjustmentOver(100);

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 90)));	// 90 : 70
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(90));

	// remaining: 90 - 70 = 20
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 70)));	// 90
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(90));
}

TEST_F(TemperatureDependentSchedulerTest, trim5) {
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 33.0f)));
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 29.0f, 0)));

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 2, 4, 0, 0), toLocalTime(2019, 8, 3, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 37.0f)));

	scheduler->trimAdjustmentOver(100);

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 90)));	// 90 : 70
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(90));

	// remaining: 90 - 70 = 20
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 80)));	// 110
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));
}

TEST_F(TemperatureDependentSchedulerTest, trim6) {
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 33.0f)));
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 29.0f, 0)));

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 2, 4, 0, 0), toLocalTime(2019, 8, 3, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 41.0f)));

	scheduler->trimAdjustmentOver(100);

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 90)));	// 90 : 70
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(90));

	// remaining: 90 - 70 = 20
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 80)));	// 130
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));
}

TEST_F(TemperatureDependentSchedulerTest, trim7) {
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 36.0f)));
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 38.0f, 0)));

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 2, 4, 0, 0), toLocalTime(2019, 8, 3, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 37.0f)));
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 2, 4, 0, 0), toLocalTime(2019, 8, 3, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 38.0f, 0)));

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 3, 4, 0, 0), toLocalTime(2019, 8, 4, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 37.0f)));
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 3, 4, 0, 0), toLocalTime(2019, 8, 4, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 35.0f, 0)));

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 4, 4, 0, 0), toLocalTime(2019, 8, 5, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 36.0f)));
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 4, 4, 0, 0), toLocalTime(2019, 8, 5, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 37.0f, 0)));

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 5, 4, 0, 0), toLocalTime(2019, 8, 6, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 34.0f)));
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 5, 4, 0, 0), toLocalTime(2019, 8, 6, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 21.0f, 0)));

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 6, 4, 0, 0), toLocalTime(2019, 8, 7, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 30.0f)));


	scheduler->trimAdjustmentOver(100);

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 105 : 115
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	// remaining: 100 - 100 = 0
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 110 : 115
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	// remaining: 100 - 100 = 0
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 3, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 110 : 100
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	// remaining: 100 - 100 = 0
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 4, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 105 : 110
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	// remaining: 100 - 100 = 0
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 5, 4, 0, 0)), Eq(Scheduler::Result(true, true, 95)));	// 95 : 40
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(95));

	// remaining: 95 - 40 = 55
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 6, 4, 0, 0)), Eq(Scheduler::Result(true, true, 20)));	// 75
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(75));
}

TEST_F(TemperatureDependentSchedulerTest, trim8) {
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 36.0f)));
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 38.0f, 0)));

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 2, 4, 0, 0), toLocalTime(2019, 8, 3, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 36.0f)));
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 2, 4, 0, 0), toLocalTime(2019, 8, 3, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 38.0f, 0)));

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 3, 4, 0, 0), toLocalTime(2019, 8, 4, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 36.0f)));

	scheduler->trimAdjustmentOver(100);

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 105 : 115
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	// remaining: 100 - 100 = 0
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 105 : 115
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	// remaining: 100 - 100 = 0
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 3, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 105
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));
}

TEST_F(TemperatureDependentSchedulerTest, trim9) {
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 38.0f)));
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 36.0f, 0)));

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 2, 4, 0, 0), toLocalTime(2019, 8, 3, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 38.0f)));
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 2, 4, 0, 0), toLocalTime(2019, 8, 3, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 36.0f, 0)));

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 3, 4, 0, 0), toLocalTime(2019, 8, 4, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 38.0f)));

	scheduler->trimAdjustmentOver(100);

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 115 : 105
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	// remaining: 100 - 100 = 0
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 115 : 105
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	// remaining: 100 - 100 = 0
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 3, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 115
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));
}

TEST_F(TemperatureDependentSchedulerTest, trim11) {
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 30.0f)));
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 38.0f, 0)));

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 2, 4, 0, 0), toLocalTime(2019, 8, 3, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 30.0f)));
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 2, 4, 0, 0), toLocalTime(2019, 8, 3, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 38.0f, 0)));

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 3, 4, 0, 0), toLocalTime(2019, 8, 4, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 30.0f)));

	scheduler->trimAdjustmentOver(100);

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 75)));	// 75 : 115
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(75));

	// remaining: 75 - 100 = -25
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 75 : 115
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(75));

	// remaining: 75 - 100 = -25
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 3, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 75
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(75));
}

TEST_F(TemperatureDependentSchedulerTest, trim12) {
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 38.0f)));
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 30.0f, 0)));

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 2, 4, 0, 0), toLocalTime(2019, 8, 3, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 38.0f)));
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 2, 4, 0, 0), toLocalTime(2019, 8, 3, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 30.0f, 0)));

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 3, 4, 0, 0), toLocalTime(2019, 8, 4, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 38.0f)));
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 3, 4, 0, 0), toLocalTime(2019, 8, 4, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 30.0f, 0)));

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 4, 4, 0, 0), toLocalTime(2019, 8, 5, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 38.0f)));

	scheduler->trimAdjustmentOver(100);

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 115 : 75
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	// remaining: 100 - 75 = 25
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 75)));	// 115 : 75
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	// remaining: 100 - 75 = 25
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 3, 4, 0, 0)), Eq(Scheduler::Result(true, true, 75)));	// 115 : 75
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	// remaining: 100 - 75 = 25
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 4, 4, 0, 0)), Eq(Scheduler::Result(true, true, 75)));	// 115
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));
}
