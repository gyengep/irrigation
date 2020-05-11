#include "HotWeatherSchedulerTest.h"
#include "TemperatureDependentSchedulerTest.h"
#include "Utils/TimeConversion.h"
#include "Logger/Logger.h"

using namespace std;
using namespace testing;


void HotWeatherSchedulerTest::SetUp() {
	//LOGGER.setLevel(LogLevel::TRACE);
	//LOGGER.setOutputStream(cout);

	mockTemperatureHistory = make_shared<MockTemperatureHistory>();
	scheduler.reset(new HotWeatherScheduler(mockTemperatureHistory));
}

void HotWeatherSchedulerTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(HotWeatherSchedulerTest, process) {
	scheduler->setMinTemperature(25);
	scheduler->setPeriod(chrono::hours(2));

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(fromLocalTime(2019, 8, 7, 12, 0, 0), fromLocalTime(2019, 8, 7, 14, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 25)));

	EXPECT_THAT(scheduler->process(fromLocalTime(2019, 8, 7, 14, 0, 0)), Eq(Scheduler::Result(true, false, 0)));
}

TEST_F(HotWeatherSchedulerTest, processHighTemp) {
	scheduler->setMinTemperature(25);
	scheduler->setPeriod(chrono::hours(2));

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(fromLocalTime(2019, 8, 7, 12, 0, 0), fromLocalTime(2019, 8, 7, 14, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 26)));

	EXPECT_THAT(scheduler->process(fromLocalTime(2019, 8, 7, 14, 0, 0)), Eq(Scheduler::Result(true, false, 0)));
}

TEST_F(HotWeatherSchedulerTest, processLowTemp) {
	scheduler->setMinTemperature(25);
	scheduler->setPeriod(chrono::hours(2));

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(fromLocalTime(2019, 8, 7, 12, 0, 0), fromLocalTime(2019, 8, 7, 14, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 24)));

	EXPECT_THAT(scheduler->process(fromLocalTime(2019, 8, 7, 14, 0, 0)), Eq(Scheduler::Result(false, false, 0)));
}

TEST_F(HotWeatherSchedulerTest, period1Hours) {
	scheduler->setPeriod(chrono::hours(1));

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(fromLocalTime(2019, 8, 7, 13, 0, 0), fromLocalTime(2019, 8, 7, 14, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 26)));

	scheduler->process(fromLocalTime(2019, 8, 7, 14, 0, 0));
}

TEST_F(HotWeatherSchedulerTest, period3Hours) {
	scheduler->setPeriod(chrono::hours(3));

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(fromLocalTime(2019, 8, 7, 11, 0, 0), fromLocalTime(2019, 8, 7, 14, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 26)));

	scheduler->process(fromLocalTime(2019, 8, 7, 14, 0, 0));
}

TEST_F(HotWeatherSchedulerTest, lastRun1Hour) {
	scheduler->setPeriod(chrono::hours(1));
	scheduler->setMinTemperature(25);

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(fromLocalTime(2019, 8, 7, 13, 0, 0), fromLocalTime(2019, 8, 7, 14, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 26)));
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(fromLocalTime(2019, 8, 7, 14, 0, 0), fromLocalTime(2019, 8, 7, 15, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 27)));
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(fromLocalTime(2019, 8, 7, 15, 0, 0), fromLocalTime(2019, 8, 7, 16, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 28)));

	EXPECT_THAT(scheduler->process(fromLocalTime(2019, 8, 7, 14, 0, 0)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler->process(fromLocalTime(2019, 8, 7, 15, 0, 0)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler->process(fromLocalTime(2019, 8, 7, 16, 0, 0)), Eq(Scheduler::Result(true, false, 0)));
}

TEST_F(HotWeatherSchedulerTest, lastRun2Hour) {
	scheduler->setPeriod(chrono::hours(2));
	scheduler->setMinTemperature(25);

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(fromLocalTime(2019, 8, 7, 12, 0, 0), fromLocalTime(2019, 8, 7, 14, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 26)));
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(fromLocalTime(2019, 8, 7, 13, 0, 0), fromLocalTime(2019, 8, 7, 15, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 27)));
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(fromLocalTime(2019, 8, 7, 14, 0, 0), fromLocalTime(2019, 8, 7, 16, 0, 0))).
		WillOnce(Return(TemperatureHistory::Values(20, 30, 28)));

	EXPECT_THAT(scheduler->process(fromLocalTime(2019, 8, 7, 14, 0, 0)), Eq(Scheduler::Result(true, false, 0)));
	EXPECT_THAT(scheduler->process(fromLocalTime(2019, 8, 7, 15, 0, 0)), Eq(Scheduler::Result(false, false, 0)));
	EXPECT_THAT(scheduler->process(fromLocalTime(2019, 8, 7, 16, 0, 0)), Eq(Scheduler::Result(true, false, 0)));
}

