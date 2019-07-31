#include "FixedAmountSchedulerTest.h"
#include "Utils/TimeConversion.h"

using namespace std;
using namespace testing;

void FixedAmountSchedulerTest::SetUp() {
	mockTemperatureForecast = make_shared<MockTemperatureForecast>();
	mockTemperatureHistory = make_shared<MockTemperatureHistory>();
	scheduler.reset(new FixedAmountScheduler(mockTemperatureForecast, mockTemperatureHistory));

	scheduler->setTemperatureAndPercents(vector<pair<float, int>>{
		{ 10.0f, 20 },
		{ 15.0f, 40 },
		{ 20.0f, 60 },
		{ 25.0f, 80 },
		{ 30.0f, 100 }
	});
}

void FixedAmountSchedulerTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

time_t toLocalTime(int year, int month, int day, int hour, int min, int sec) {
	tm timeinfo = toCalendarTime(year, month, day, hour, min, sec);
	return timelocal(&timeinfo);
}

#include "Logger/Logger.h"

TEST_F(FixedAmountSchedulerTest, getAdjustment) {
	LOGGER.setLevel(LogLevel::TRACE);
	LOGGER.setOutputStream(cout);

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 25)));

	scheduler->process(toCalendarTime(2020, 2, 28, 4, 0, 0));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 28, 4, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));
}

TEST_F(FixedAmountSchedulerTest, getAdjustmentWithoutCorrection1) {
	LOGGER.setLevel(LogLevel::TRACE);
	LOGGER.setOutputStream(cout);

	const int temp1 = 20;
	const int temp2 = 12;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, temp1)));
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 29, 4, 0, 0), toLocalTime(2020, 3, 1, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, temp2)));

	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, temp1, 0)));

	scheduler->process(toCalendarTime(2020, 2, 28, 4, 0, 0));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 28, 4, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	scheduler->process(toCalendarTime(2020, 2, 29, 4, 0, 0));
	EXPECT_FALSE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 29, 4, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(0));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(40));
}

TEST_F(FixedAmountSchedulerTest, getAdjustmentWithCorrection1) {
	LOGGER.setLevel(LogLevel::TRACE);
	LOGGER.setOutputStream(cout);

	const int temp1 = 20;
	const int correction1 = 5;
	const int temp2 = 12;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, temp1)));
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 29, 4, 0, 0), toLocalTime(2020, 3, 1, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, temp2)));

	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, temp1 + correction1, 0)));

	scheduler->process(toCalendarTime(2020, 2, 28, 4, 0, 0));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 28, 4, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	scheduler->process(toCalendarTime(2020, 2, 29, 4, 0, 0));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 29, 4, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(120));
}

TEST_F(FixedAmountSchedulerTest, getAdjustmentWithoutCorrection2) {
	LOGGER.setLevel(LogLevel::TRACE);
	LOGGER.setOutputStream(cout);

	const int temp1 = 15;
	const int temp2 = 22;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, temp1)));
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 29, 4, 0, 0), toLocalTime(2020, 3, 1, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, temp2)));

	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, temp1, 0)));

	scheduler->process(toCalendarTime(2020, 2, 28, 4, 0, 0));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 28, 4, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	scheduler->process(toCalendarTime(2020, 2, 29, 4, 0, 0));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 29, 4, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(160));
}

TEST_F(FixedAmountSchedulerTest, getAdjustmentWithCorrection2) {
	LOGGER.setLevel(LogLevel::TRACE);
	LOGGER.setOutputStream(cout);

	const int temp1 = 15;
	const int correction1 = -5;
	const int temp2 = 22;

	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, temp1)));
	EXPECT_CALL(*mockTemperatureForecast, getForecastValues(toLocalTime(2020, 2, 29, 4, 0, 0), toLocalTime(2020, 3, 1, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, temp2)));

	EXPECT_CALL(*mockTemperatureHistory, getHistoryValues(toLocalTime(2020, 2, 28, 4, 0, 0), toLocalTime(2020, 2, 29, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, temp1 + correction1, 0)));

	scheduler->process(toCalendarTime(2020, 2, 28, 4, 0, 0));
	EXPECT_TRUE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 28, 4, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(100));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(100));

	scheduler->process(toCalendarTime(2020, 2, 29, 4, 0, 0));
	EXPECT_FALSE(scheduler->isDayScheduled(toCalendarTime(2020, 2, 29, 4, 0, 0)));
	EXPECT_THAT(scheduler->getAdjustment(), Eq(0));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(80));
}
