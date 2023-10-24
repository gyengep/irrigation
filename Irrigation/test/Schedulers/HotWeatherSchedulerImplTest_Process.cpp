#include "HotWeatherSchedulerImplTest.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST_F(HotWeatherSchedulerImplProcessTest, process) {
	hotWeatherScheduler->setMinTemperature(25);
	hotWeatherScheduler->setPeriod(std::chrono::hours(2));

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 7, 12, 0, 0), LocalDateTime(2019, 8, 7, 14, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 25)));

	EXPECT_THAT(hotWeatherScheduler->process(LocalDateTime(2019, 8, 7, 14, 0, 0)), Pointee(Scheduler::Result(true)));
}

TEST_F(HotWeatherSchedulerImplProcessTest, processHighTemp) {
	hotWeatherScheduler->setMinTemperature(25);
	hotWeatherScheduler->setPeriod(std::chrono::hours(2));

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 7, 12, 0, 0), LocalDateTime(2019, 8, 7, 14, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 26)));

	EXPECT_THAT(hotWeatherScheduler->process(LocalDateTime(2019, 8, 7, 14, 0, 0)), Pointee(Scheduler::Result(true)));
}

TEST_F(HotWeatherSchedulerImplProcessTest, processLowTemp) {
	hotWeatherScheduler->setMinTemperature(25);
	hotWeatherScheduler->setPeriod(std::chrono::hours(2));

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 7, 12, 0, 0), LocalDateTime(2019, 8, 7, 14, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 24)));

	EXPECT_THAT(hotWeatherScheduler->process(LocalDateTime(2019, 8, 7, 14, 0, 0)), Pointee(Scheduler::Result(false)));
}

TEST_F(HotWeatherSchedulerImplProcessTest, period1Hours) {
	hotWeatherScheduler->setPeriod(std::chrono::hours(1));

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 7, 13, 0, 0), LocalDateTime(2019, 8, 7, 14, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 26)));

	hotWeatherScheduler->process(LocalDateTime(2019, 8, 7, 14, 0, 0));
}

TEST_F(HotWeatherSchedulerImplProcessTest, period3Hours) {
	hotWeatherScheduler->setPeriod(std::chrono::hours(3));

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 7, 11, 0, 0), LocalDateTime(2019, 8, 7, 14, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 26)));

	hotWeatherScheduler->process(LocalDateTime(2019, 8, 7, 14, 0, 0));
}

TEST_F(HotWeatherSchedulerImplProcessTest, lastRun1Hour) {
	hotWeatherScheduler->setPeriod(std::chrono::hours(1));
	hotWeatherScheduler->setMinTemperature(25);

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 7, 13, 0, 0), LocalDateTime(2019, 8, 7, 14, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 26)));
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 7, 14, 0, 0), LocalDateTime(2019, 8, 7, 15, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 27)));
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 7, 15, 0, 0), LocalDateTime(2019, 8, 7, 16, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 28)));

	EXPECT_THAT(hotWeatherScheduler->process(LocalDateTime(2019, 8, 7, 14, 0, 0)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(hotWeatherScheduler->process(LocalDateTime(2019, 8, 7, 15, 0, 0)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(hotWeatherScheduler->process(LocalDateTime(2019, 8, 7, 16, 0, 0)), Pointee(Scheduler::Result(true)));
}

TEST_F(HotWeatherSchedulerImplProcessTest, lastRun2Hour) {
	hotWeatherScheduler->setPeriod(std::chrono::hours(2));
	hotWeatherScheduler->setMinTemperature(25);

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 7, 12, 0, 0), LocalDateTime(2019, 8, 7, 14, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 26)));
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 7, 13, 0, 0), LocalDateTime(2019, 8, 7, 15, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 27)));
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 7, 14, 0, 0), LocalDateTime(2019, 8, 7, 16, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 28)));

	EXPECT_THAT(hotWeatherScheduler->process(LocalDateTime(2019, 8, 7, 14, 0, 0)), Pointee(Scheduler::Result(true)));
	EXPECT_THAT(hotWeatherScheduler->process(LocalDateTime(2019, 8, 7, 15, 0, 0)), Pointee(Scheduler::Result(false)));
	EXPECT_THAT(hotWeatherScheduler->process(LocalDateTime(2019, 8, 7, 16, 0, 0)), Pointee(Scheduler::Result(true)));
}

