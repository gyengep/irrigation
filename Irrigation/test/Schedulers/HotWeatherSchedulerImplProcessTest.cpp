#include "HotWeatherSchedulerImplProcessTest.h"
#include "Utils/TimeConversion.h"

using namespace testing;


void HotWeatherSchedulerImplProcessTest::SetUp() {
	mockTemperatureHistory = std::make_shared<MockTemperatureHistory>();
	scheduler = std::make_shared<HotWeatherSchedulerImpl>(mockTemperatureHistory);
}

void HotWeatherSchedulerImplProcessTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(HotWeatherSchedulerImplProcessTest, process) {
	scheduler->setMinTemperature(25);
	scheduler->setPeriod(std::chrono::hours(2));

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 7, 12, 0, 0), LocalDateTime(2019, 8, 7, 14, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 25)));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 7, 14, 0, 0)), Eq(Scheduler::Result(true, false, 0)));
}

TEST_F(HotWeatherSchedulerImplProcessTest, processHighTemp) {
	scheduler->setMinTemperature(25);
	scheduler->setPeriod(std::chrono::hours(2));

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 7, 12, 0, 0), LocalDateTime(2019, 8, 7, 14, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 26)));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 7, 14, 0, 0)), Eq(Scheduler::Result(true, false, 0)));
}

TEST_F(HotWeatherSchedulerImplProcessTest, processLowTemp) {
	scheduler->setMinTemperature(25);
	scheduler->setPeriod(std::chrono::hours(2));

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 7, 12, 0, 0), LocalDateTime(2019, 8, 7, 14, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 24)));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 7, 14, 0, 0)), Eq(Scheduler::Result(false, false, 0)));
}

TEST_F(HotWeatherSchedulerImplProcessTest, period1Hours) {
	scheduler->setPeriod(std::chrono::hours(1));

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 7, 13, 0, 0), LocalDateTime(2019, 8, 7, 14, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 26)));

	scheduler->process(LocalDateTime(2019, 8, 7, 14, 0, 0));
}

TEST_F(HotWeatherSchedulerImplProcessTest, period3Hours) {
	scheduler->setPeriod(std::chrono::hours(3));

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 7, 11, 0, 0), LocalDateTime(2019, 8, 7, 14, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 26)));

	scheduler->process(LocalDateTime(2019, 8, 7, 14, 0, 0));
}

TEST_F(HotWeatherSchedulerImplProcessTest, lastRun1Hour) {
	scheduler->setPeriod(std::chrono::hours(1));
	scheduler->setMinTemperature(25);

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 7, 13, 0, 0), LocalDateTime(2019, 8, 7, 14, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 26)));
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 7, 14, 0, 0), LocalDateTime(2019, 8, 7, 15, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 27)));
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 7, 15, 0, 0), LocalDateTime(2019, 8, 7, 16, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 28)));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 7, 14, 0, 0)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 7, 15, 0, 0)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 7, 16, 0, 0)), Eq(Scheduler::Result(true, false, 0)));
}

TEST_F(HotWeatherSchedulerImplProcessTest, lastRun2Hour) {
	scheduler->setPeriod(std::chrono::hours(2));
	scheduler->setMinTemperature(25);

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 7, 12, 0, 0), LocalDateTime(2019, 8, 7, 14, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 26)));
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 7, 13, 0, 0), LocalDateTime(2019, 8, 7, 15, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 27)));
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 7, 14, 0, 0), LocalDateTime(2019, 8, 7, 16, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 28)));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 7, 14, 0, 0)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 7, 15, 0, 0)), Eq(Scheduler::Result(false, false, 0)));
	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 7, 16, 0, 0)), Eq(Scheduler::Result(true, false, 0)));
}

