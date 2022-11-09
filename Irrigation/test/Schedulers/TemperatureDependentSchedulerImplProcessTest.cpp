#include "TemperatureDependentSchedulerImplProcessTest.h"
#include "Schedulers/TemperatureToPercent.h"
#include "Utils/TimeConversion.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void TemperatureDependentSchedulerImplProcessTest::SetUp() {

	mockTemperatureForecast = std::make_shared<MockTemperatureForecast>();
	mockTemperatureHistory = std::make_shared<MockTemperatureHistory>();
	scheduler = std::make_shared<TemperatureDependentSchedulerImpl>(mockTemperatureForecast, mockTemperatureHistory);

	scheduler->setRemainingCorrection(1.0f);
	scheduler->setMinAdjustment(0);
	scheduler->setMaxAdjustment(0);

	TemperatureToPercent::getInstance().setTemperatureAndPercents(std::vector<std::pair<float, unsigned>>{
		{ 15.0f, 25U },
		{ 25.0f, 50U },
		{ 35.0f, 100U }
	});

	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(LocalDateTime(2019, 8, 1, 4, 0, 0), LocalDateTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 32.0f)));
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(LocalDateTime(2019, 8, 2, 4, 0, 0), LocalDateTime(2019, 8, 3, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 28.0f)));
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(LocalDateTime(2019, 8, 3, 4, 0, 0), LocalDateTime(2019, 8, 4, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 34.0f)));
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(LocalDateTime(2019, 8, 4, 4, 0, 0), LocalDateTime(2019, 8, 5, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 27.0f)));
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(LocalDateTime(2019, 8, 5, 4, 0, 0), LocalDateTime(2019, 8, 6, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 28.0f)));
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(LocalDateTime(2019, 8, 6, 4, 0, 0), LocalDateTime(2019, 8, 7, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 14.0f)));
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(LocalDateTime(2019, 8, 7, 4, 0, 0), LocalDateTime(2019, 8, 8, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 29.0f)));
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(LocalDateTime(2019, 8, 8, 4, 0, 0), LocalDateTime(2019, 8, 9, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 31.0f)));

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 1, 4, 0, 0), LocalDateTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 33.0f, 0)));
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 2, 4, 0, 0), LocalDateTime(2019, 8, 3, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 30.0f, 0)));
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 3, 4, 0, 0), LocalDateTime(2019, 8, 4, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 37.0f, 0)));
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 4, 4, 0, 0), LocalDateTime(2019, 8, 5, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 25.0f, 0)));
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 5, 4, 0, 0), LocalDateTime(2019, 8, 6, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 23.0f, 0)));
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 6, 4, 0, 0), LocalDateTime(2019, 8, 7, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 10.0f, 0)));
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 7, 4, 0, 0), LocalDateTime(2019, 8, 8, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 29.0f, 0)));
}

void TemperatureDependentSchedulerImplProcessTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(TemperatureDependentSchedulerImplProcessTest, process) {
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 85)));	// 85 : 90
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(85));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 70)));	// 65 : 75
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(65));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 3, 4, 0, 0)), Eq(Scheduler::Result(true, true, 105)));	// 95 : 110
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(95));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 4, 4, 0, 0)), Eq(Scheduler::Result(true, true, 75)));	// 60 : 50
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(60));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 5, 4, 0, 0)), Eq(Scheduler::Result(true, true, 55)));	// 65 : 45
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(65));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 6, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 0 : 0
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(20));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 7, 4, 0, 0)), Eq(Scheduler::Result(true, true, 50)));	// 70 : 70
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(70));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 8, 4, 0, 0)), Eq(Scheduler::Result(true, true, 80)));	//	80
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(80));
}

///////////////////////////////////////////////////////////////////////////////
// Min adjustment

TEST_F(TemperatureDependentSchedulerImplProcessTest, minAdjustment70) {
	scheduler->setMinAdjustment(70);

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 85)));	// 85 : 90
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(85));

	// remaining: 85 - 85 = 0
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 70)));	// 65 : 75
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(65));

	// remaining: 65 - 75 = -10
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 3, 4, 0, 0)), Eq(Scheduler::Result(true, true, 105))); 	// 95 : 110
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(95));

	// remaining: 95 - 110 = -15
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 4, 4, 0, 0)), Eq(Scheduler::Result(true, true, 75)));	// 60 : 50
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(60));

	// remaining: 60 - 50 = 10
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 5, 4, 0, 0)), Eq(Scheduler::Result(true, true, 70)));	// 65 : 45
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(80));

	// remaining: 80 - 45 = 35
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 6, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 0 : 0
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(35));

	// remaining: 35 - 0 = 35
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 7, 4, 0, 0)), Eq(Scheduler::Result(true, true, 70)));	// 70 : 70
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(105));

	// remaining: 105 - 70 = 35
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 8, 4, 0, 0)), Eq(Scheduler::Result(true, true, 70)));	//	80
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(105));
}

TEST_F(TemperatureDependentSchedulerImplProcessTest, minAdjustment100) {
	scheduler->setMinAdjustment(100);

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 85 : 90
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 65 : 75
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(110));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 3, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100))); 	// 95 : 110
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(135));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 4, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 60 : 50
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(125));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 5, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 65 : 45
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(75));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 6, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 0 : 0
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(30));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 7, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 70 : 70
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(130));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 8, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	//	80
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(160));
}

///////////////////////////////////////////////////////////////////////////////
// Max adjustment

TEST_F(TemperatureDependentSchedulerImplProcessTest, maxAdjustment) {
	scheduler->setMaxAdjustment(75);

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 75)));	// 85 : 90
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(75));

	// remaining: 75 - 90 = -15
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 75)));	// 65 : 75
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(60));

	// remaining: 60 - 75 = -15
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 3, 4, 0, 0)), Eq(Scheduler::Result(true, true, 75)));	// 95 : 110
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(60));

	// remaining: 60 - 110 = -50
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 4, 4, 0, 0)), Eq(Scheduler::Result(true, true, 75)));	// 60 : 50
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(25));

	// remaining: 25 - 50 = -25
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 5, 4, 0, 0)), Eq(Scheduler::Result(true, true, 75)));	// 65 : 45
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(50));

	// remaining: 50 - 45 = 5
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 6, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 0 : 0
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(5));

	// remaining: 5 - 0 = 5
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 7, 4, 0, 0)), Eq(Scheduler::Result(true, true, 65)));	// 70 : 70
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(70));

	// remaining: 70 - 70 = 0
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 8, 4, 0, 0)), Eq(Scheduler::Result(true, true, 75)));	//	80
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(75));
}

TEST_F(TemperatureDependentSchedulerImplProcessTest, maxAdjustmentDailyTwoTimes) {
	scheduler->setMaxAdjustment(75);

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 75)));	// 85 : 90
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(75));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 1, 20, 0, 0)), Eq(Scheduler::Result(true, true, 10)));	// 85 : 90
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(85));

	// remaining: 85 - 90 = -5
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 70)));	// 65 : 75
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(65));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 2, 20, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 65 : 75
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(65));

	// remaining: 65 - 75 = -10
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 3, 4, 0, 0)), Eq(Scheduler::Result(true, true, 75)));	// 95 : 110
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(65));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 3, 20, 0, 0)), Eq(Scheduler::Result(true, true, 30)));	// 95 : 110
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(95));

	// remaining: 95 - 110 = -15
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 4, 4, 0, 0)), Eq(Scheduler::Result(true, true, 75)));	// 60 : 50
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(60));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 4, 20, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 60 : 50
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(60));

	// remaining: 60 - 50 = 10
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 5, 4, 0, 0)), Eq(Scheduler::Result(true, true, 55)));	// 65 : 45
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(65));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 5, 20, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 65 : 45
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(65));

	// remaining: 65 - 45 = 20
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 6, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 0 : 0
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(20));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 6, 20, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 0 : 0
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(20));

	// remaining: 20 - 0 = 20
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 7, 4, 0, 0)), Eq(Scheduler::Result(true, true, 50)));	// 70 : 70
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(70));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 7, 20, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 70 : 70
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(70));

	// remaining: 70 - 70 = 0
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 8, 4, 0, 0)), Eq(Scheduler::Result(true, true, 75)));	//	80
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(75));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 8, 20, 0, 0)), Eq(Scheduler::Result(true, true, 5)));	//	80
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(80));
}

///////////////////////////////////////////////////////////////////////////////
// Min/Max adjustment

TEST_F(TemperatureDependentSchedulerImplProcessTest, minMaxAdjustment) {
	scheduler->setMinAdjustment(75);
	scheduler->setMaxAdjustment(75);

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 75)));	// 85 : 90
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(75));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 1, 16, 0, 0)), Eq(Scheduler::Result(true, true, 75)));	// 85 : 90
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(150));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 75)));	// 65 : 75
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(135));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 2, 16, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 65 : 75
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(135));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 3, 4, 0, 0)), Eq(Scheduler::Result(true, true, 75))); 	// 95 : 110
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(135));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 3, 16, 0, 0)), Eq(Scheduler::Result(false, true, 0))); 	// 95 : 110
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(135));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 4, 4, 0, 0)), Eq(Scheduler::Result(true, true, 75)));	// 60 : 50
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 4, 16, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 60 : 50
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 5, 4, 0, 0)), Eq(Scheduler::Result(true, true, 75)));	// 65 : 45
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(125));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 5, 16, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 65 : 45
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(125));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 6, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 0 : 0
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(80));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 6, 16, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 0 : 0
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(80));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 7, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 70 : 70
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(80));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 7, 16, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 70 : 70
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(80));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 8, 4, 0, 0)), Eq(Scheduler::Result(true, true, 75)));	//	80
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(85));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 8, 16, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	//	80
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(85));
}

///////////////////////////////////////////////////////////////////////////////
// Remaining correction

TEST_F(TemperatureDependentSchedulerImplProcessTest, remainingCorrection100) {
	scheduler->setRemainingCorrection(1.0f);

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 85)));	// 85 : 90
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(85));

	//	remaining: 85 - 90 = -5
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 70)));	// 65 : 75
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(65));

	//	remaining: 65 - 75 = -10
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 3, 4, 0, 0)), Eq(Scheduler::Result(true, true, 105)));	// 95 : 110
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(95));

	//	remaining: 95 - 110 = -15
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 4, 4, 0, 0)), Eq(Scheduler::Result(true, true, 75)));	// 60 : 50
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(60));

	//	remaining: 60 - 50 = 10
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 5, 4, 0, 0)), Eq(Scheduler::Result(true, true, 55)));	// 65 : 45
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(65));

	//	remaining: 65 - 45 = 20
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 6, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 0 : 0
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(20));

	//	remaining: 20 - 0 = 20
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 7, 4, 0, 0)), Eq(Scheduler::Result(true, true, 50)));	// 70 : 70
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(70));

	//	remaining: 70 - 70 = 0
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 8, 4, 0, 0)), Eq(Scheduler::Result(true, true, 80)));	//	80
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(80));
}

TEST_F(TemperatureDependentSchedulerImplProcessTest, remainingCorrection50) {
	scheduler->setRemainingCorrection(0.5f);

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 85)));	// 85 : 90
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(85));

	//	remaining: 85 - 90 = -5
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 67)));	// 65 : 75
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(65));

	//	remaining: 65 - 75 = -10
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 3, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 95 : 110
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(95));

	//	remaining: 95 - 110 = -15
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 4, 4, 0, 0)), Eq(Scheduler::Result(true, true, 67)));	// 60 : 50
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(60));

	//	remaining: 60 - 50 = 10
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 5, 4, 0, 0)), Eq(Scheduler::Result(true, true, 60)));	// 65 : 45
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(65));

	//	remaining: 65 - 45 = 20
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 6, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 0 : 0
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(10));

	//	remaining: 10 - 0 = 10
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 7, 4, 0, 0)), Eq(Scheduler::Result(true, true, 65)));	// 70 : 70
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(70));

	//	remaining: 70 - 70 = 0
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 8, 4, 0, 0)), Eq(Scheduler::Result(true, true, 80)));	//	80
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(80));
}

TEST_F(TemperatureDependentSchedulerImplProcessTest, remainingCorrection0) {
	scheduler->setRemainingCorrection(0.0f);

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 85)));	// 85 : 90
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(85));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 65)));	// 65 : 75
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(65));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 3, 4, 0, 0)), Eq(Scheduler::Result(true, true, 95)));	// 95 : 110
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(95));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 4, 4, 0, 0)), Eq(Scheduler::Result(true, true, 60)));	// 60 : 50
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(60));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 5, 4, 0, 0)), Eq(Scheduler::Result(true, true, 65)));	// 65 : 45
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(65));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 6, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 0 : 0
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(0));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 7, 4, 0, 0)), Eq(Scheduler::Result(true, true, 70)));	// 70 : 70
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(70));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 8, 4, 0, 0)), Eq(Scheduler::Result(true, true, 80)));	//	80
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(80));
}

///////////////////////////////////////////////////////////////////////////////
// Min adjustment / Remaining correction

TEST_F(TemperatureDependentSchedulerImplProcessTest, minAdjustmentAndRemainingCorrection100) {
 	scheduler->setMinAdjustment(100);
	scheduler->setRemainingCorrection(1.0f);

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 85 : 90
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	//	remaining: 100 - 90 = 10
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 65 : 75
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(110));

	//	remaining: 110 - 75 = 35
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 3, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100))); 	// 95 : 110
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(135));

	//	remaining: 135 - 110 = 25
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 4, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 60 : 50
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(125));

	//	remaining: 125 - 50 = 75
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 5, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 65 : 45
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(75));

	//	remaining: 75 - 45 = 30
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 6, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 0 : 0
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(30));

	//	remaining: 30 - 0 = 30
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 7, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 70 : 70
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(130));

	//	remaining: 130 - 70 = 60
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 8, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	//	80
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(160));
}

TEST_F(TemperatureDependentSchedulerImplProcessTest, minAdjustmentAndRemainingCorrection50) {
	scheduler->setMinAdjustment(100);
	scheduler->setRemainingCorrection(0.5f);

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 85 : 90
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	//	remaining: 100 - 90 = 10
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 65 : 75
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(105));

	//	remaining: 105 - 75 = 30
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 3, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100))); 	// 95 : 110
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(115));

	//	remaining: 115 - 110 = 5
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 4, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 60 : 50
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(102));

	//	remaining: 102 - 50 = 52
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 5, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 65 : 45
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(126));

	//	remaining: 126 - 45 = 81
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 6, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 0 : 0
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(40));

	//	remaining: 40 - 0 = 40
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 7, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 70 : 70
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(120));

	//	remaining: 120 - 70 = 50
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 8, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	//	80
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(125));
}

TEST_F(TemperatureDependentSchedulerImplProcessTest, minAdjustmentAndRemainingCorrection0) {
	scheduler->setMinAdjustment(100);
	scheduler->setRemainingCorrection(0.0f);

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 1, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 85 : 90
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 2, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 65 : 75
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 3, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100))); 	// 95 : 110
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 4, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 60 : 50
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 5, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 65 : 45
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 6, 4, 0, 0)), Eq(Scheduler::Result(false, true, 0)));	// 0 : 0
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(0));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 7, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	// 70 : 70
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 8, 4, 0, 0)), Eq(Scheduler::Result(true, true, 100)));	//	80
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));
}
