#include "FixedAmountSchedulerTest.h"
#include "Schedulers/TemperatureToPercent.h"
#include "Utils/TimeConversion.h"
#include "Logger/Logger.h"

using namespace std;
using namespace testing;


time_t toLocalTime(int year, int month, int day, int hour, int min, int sec) {
	struct tm timeinfo = toCalendarTime(year, month, day, hour, min, sec);
	return timelocal(&timeinfo);
}

void FixedAmountSchedulerTest::SetUp() {
	//LOGGER.setLevel(LogLevel::TRACE);
	//LOGGER.setOutputStream(cout);

	mockTemperatureForecast = make_shared<MockTemperatureForecast>();
	mockTemperatureHistory = make_shared<MockTemperatureHistory>();
	scheduler.reset(new FixedAmountScheduler(mockTemperatureForecast, mockTemperatureHistory));

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

void FixedAmountSchedulerTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(FixedAmountSchedulerTest, process) {
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 85 : 90
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 65 : 75
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(110));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 3, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100))); 	// 95 : 110
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(135));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 4, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 60 : 50
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(125));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 5, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 65 : 45
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(75));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 6, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 0 : 0
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(30));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 7, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 70 : 70
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(130));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 8, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	//	80
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(160));
}

TEST_F(FixedAmountSchedulerTest, getAdjustmentWith100Remaining) {
	scheduler->setRemainingCorrection(1.0f);

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 85 : 90
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	//	reaining: 100 - 90 = 10
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 65 : 75
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(110));

	//	reaining: 110 - 75 = 35
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 3, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100))); 	// 95 : 110
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(135));

	//	reaining: 135 - 110 = 25
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 4, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 60 : 50
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(125));

	//	reaining: 125 - 50 = 75
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 5, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 65 : 45
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(75));

	//	reaining: 75 - 45 = 30
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 6, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 0 : 0
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(30));

	//	reaining: 30 - 0 = 30
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 7, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 70 : 70
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(130));

	//	reaining: 130 - 70 = 60
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 8, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	//	80
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(160));
}

TEST_F(FixedAmountSchedulerTest, getAdjustmentWith50Remaining) {
	LOGGER.setLevel(LogLevel::TRACE);
	LOGGER.setOutputStream(cout);

	scheduler->setRemainingCorrection(0.5f);

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 85 : 90
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	//	reaining: 100 - 90 = 10
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 65 : 75
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(105));

	//	reaining: 105 - 75 = 30
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 3, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100))); 	// 95 : 110
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(115));

	//	reaining: 115 - 110 = 5
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 4, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 60 : 50
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(102));

	//	reaining: 102 - 50 = 52
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 5, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 65 : 45
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(126));

	//	reaining: 126 - 45 = 81
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 6, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 0 : 0
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(40));

	//	reaining: 40 - 0 = 40
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 7, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 70 : 70
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(120));

	//	reaining: 120 - 70 = 50
	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 8, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	//	80
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(125));
}

TEST_F(FixedAmountSchedulerTest, getAdjustmentWith0Remaining) {
	scheduler->setRemainingCorrection(0.0f);

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 85 : 90
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 65 : 75
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 3, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100))); 	// 95 : 110
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 4, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 60 : 50
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 5, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 65 : 45
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 6, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 0 : 0
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(0));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 7, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 70 : 70
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	EXPECT_THAT(scheduler->process(toLocalTime(2019, 8, 8, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	//	80
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));
}


/*
TEST_F(FixedAmountSchedulerTest, getAdjustment) {
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 25)));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));
}

TEST_F(FixedAmountSchedulerTest, getAdjustmentLower) {
	const int forecastedTemp1 = 9;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(0));
}

TEST_F(FixedAmountSchedulerTest, getAdjustmentHigher) {
	const int forecastedTemp1 = 35;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 4, 0, 0)), Eq(Scheduler::Result(true, true, 120)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(120));
}

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

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 29, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));
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

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 29, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(120));
}

TEST_F(FixedAmountSchedulerTest, getAdjustmentSameDay1) {
	const int forecastedTemp1 = 20;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 10, 0, 0)), Eq(Scheduler::Result(false, true, 0)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));
}

TEST_F(FixedAmountSchedulerTest, getAdjustmentEarlierDay1) {
	const int forecastedTemp1 = 20;
	const int forecastedTemp2 = 12;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 20, 4, 0, 0), toLocalTime(2020, 2, 21, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 22, 4, 0, 0), toLocalTime(2020, 2, 23, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp2)));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 20, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 22, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));
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

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 29, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));
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

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 29, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(80));
}

TEST_F(FixedAmountSchedulerTest, getAdjustmentSameDay2) {
	const int forecastedTemp1 = 15;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 10, 0, 0)), Eq(Scheduler::Result(false, true, 0)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));
}

TEST_F(FixedAmountSchedulerTest, getAdjustmentEarlierDay2) {
	const int forecastedTemp1 = 15;
	const int forecastedTemp2 = 22;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 20, 4, 0, 0), toLocalTime(2020, 2, 21, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 22, 4, 0, 0), toLocalTime(2020, 2, 23, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp2)));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 20, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 22, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(FixedAmountSchedulerTest, dayStart) {
	const int forecastedTemp1 = 15;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 0, 30, 0), toLocalTime(2020, 2, 29, 0, 29, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, forecastedTemp1)));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 0, 30, 0)), Eq(Scheduler::Result(true, true, 100)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	EXPECT_THAT(scheduler->process(toLocalTime(2020, 2, 28, 23, 30, 0)), Eq(Scheduler::Result(false, true, 0)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));
}
*/
