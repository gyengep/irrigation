#include "FixedPeriodSchedulerTest.h"
#include "Schedulers/TemperatureToPercent.h"
#include "Utils/TimeConversion.h"
#include "Logger/Logger.h"

using namespace std;
using namespace testing;

extern time_t toLocalTime(int year, int month, int day, int hour, int min, int sec);


void FixedPeriodSchedulerTest::SetUp() {
	//LOGGER.setLevel(LogLevel::TRACE);
	//LOGGER.setOutputStream(cout);

	mockTemperatureForecast = make_shared<MockTemperatureForecast>();
	mockTemperatureHistory = make_shared<MockTemperatureHistory>();
	scheduler.reset(new FixedPeriodScheduler(mockTemperatureForecast, mockTemperatureHistory));

	TemperatureToPercent::getInstance().setTemperatureAndPercents(vector<pair<float, int>>{
		{ 15.0f, 25 },
		{ 25.0f, 50 },
		{ 35.0f, 100 }
	});

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 32.0f)));
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 2, 4, 0, 0), toLocalTime(2019, 8, 3, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 28.0f)));
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 3, 4, 0, 0), toLocalTime(2019, 8, 4, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 34.0f)));
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 4, 4, 0, 0), toLocalTime(2019, 8, 5, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 27.0f)));
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 5, 4, 0, 0), toLocalTime(2019, 8, 6, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 28.0f)));
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 6, 4, 0, 0), toLocalTime(2019, 8, 7, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 14.0f)));
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 7, 4, 0, 0), toLocalTime(2019, 8, 8, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 29.0f)));
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2019, 8, 8, 4, 0, 0), toLocalTime(2019, 8, 9, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 31.0f)));

	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 1, 4, 0, 0), toLocalTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 33.0f, 0)));
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 2, 4, 0, 0), toLocalTime(2019, 8, 3, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 30.0f, 0)));
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 3, 4, 0, 0), toLocalTime(2019, 8, 4, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 37.0f, 0)));
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 4, 4, 0, 0), toLocalTime(2019, 8, 5, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 25.0f, 0)));
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 5, 4, 0, 0), toLocalTime(2019, 8, 6, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 23.0f, 0)));
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 6, 4, 0, 0), toLocalTime(2019, 8, 7, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 10.0f, 0)));
	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2019, 8, 7, 4, 0, 0), toLocalTime(2019, 8, 8, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 29.0f, 0)));
}

void FixedPeriodSchedulerTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(FixedPeriodSchedulerTest, process) {
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 85)));	// 85 : 90
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(85));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 70)));	// 65 : 75
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(65));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 3, 4, 0, 0)), Eq(Scheduler::Result(true, true, 105)));	// 95 : 110
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(95));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 4, 4, 0, 0)), Eq(Scheduler::Result(true, true, 75)));	// 60 : 50
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(60));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 5, 4, 0, 0)), Eq(Scheduler::Result(true, true, 55)));	// 65 : 45
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(65));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 6, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 0 : 0
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(20));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 7, 4, 0, 0)), Eq(Scheduler::Result(true, true, 50)));	// 70 : 70
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(70));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 8, 4, 0, 0)), Eq(Scheduler::Result(true, true, 80)));	//	80
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(80));
}

TEST_F(FixedPeriodSchedulerTest, getAdjustmentWith100Remaining) {
	scheduler->setRemainingCorrection(1.0f);

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 85)));	// 85 : 90
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(85));

	//	reaining: 85 - 90 = -5
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 70)));	// 65 : 75
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(65));

	//	reaining: 65 - 75 = -10
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 3, 4, 0, 0)), Eq(Scheduler::Result(true, true, 105)));	// 95 : 110
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(95));

	//	reaining: 95 - 110 = -15
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 4, 4, 0, 0)), Eq(Scheduler::Result(true, true, 75)));	// 60 : 50
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(60));

	//	reaining: 60 - 50 = 10
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 5, 4, 0, 0)), Eq(Scheduler::Result(true, true, 55)));	// 65 : 45
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(65));

	//	reaining: 65 - 45 = 20
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 6, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 0 : 0
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(20));

	//	reaining: 20 - 0 = 20
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 7, 4, 0, 0)), Eq(Scheduler::Result(true, true, 50)));	// 70 : 70
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(70));

	//	reaining: 70 - 70 = 0
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 8, 4, 0, 0)), Eq(Scheduler::Result(true, true, 80)));	//	80
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(80));
}

TEST_F(FixedPeriodSchedulerTest, getAdjustmentWith50Remaining) {
	scheduler->setRemainingCorrection(0.5f);

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 85)));	// 85 : 90
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(85));

	//	reaining: 85 - 90 = -5
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 67)));	// 65 : 75
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(65));

	//	reaining: 65 - 75 = -10
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 3, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 95 : 110
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(95));

	//	reaining: 95 - 110 = -15
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 4, 4, 0, 0)), Eq(Scheduler::Result(true, true, 67)));	// 60 : 50
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(60));

	//	reaining: 60 - 50 = 10
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 5, 4, 0, 0)), Eq(Scheduler::Result(true, true, 60)));	// 65 : 45
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(65));

	//	reaining: 65 - 45 = 20
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 6, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 0 : 0
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(10));

	//	reaining: 10 - 0 = 10
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 7, 4, 0, 0)), Eq(Scheduler::Result(true, true, 65)));	// 70 : 70
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(70));

	//	reaining: 70 - 70 = 0
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 8, 4, 0, 0)), Eq(Scheduler::Result(true, true, 80)));	//	80
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(80));
}

TEST_F(FixedPeriodSchedulerTest, getAdjustmentWith0Remaining) {
	scheduler->setRemainingCorrection(0.0f);

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 85)));	// 85 : 90
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(85));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 65)));	// 65 : 75
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(65));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 3, 4, 0, 0)), Eq(Scheduler::Result(true, true, 95)));	// 95 : 110
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(95));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 4, 4, 0, 0)), Eq(Scheduler::Result(true, true, 60)));	// 60 : 50
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(60));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 5, 4, 0, 0)), Eq(Scheduler::Result(true, true, 65)));	// 65 : 45
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(65));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 6, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 0 : 0
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(0));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 7, 4, 0, 0)), Eq(Scheduler::Result(true, true, 70)));	// 70 : 70
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(70));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 8, 4, 0, 0)), Eq(Scheduler::Result(true, true, 80)));	//	80
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(80));
}

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/*
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

*/
