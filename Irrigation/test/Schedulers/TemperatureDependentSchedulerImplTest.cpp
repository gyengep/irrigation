#include "TemperatureDependentSchedulerImplTest.h"
#include "Schedulers/TemperatureToPercent.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void TemperatureDependentSchedulerImplTest::SetUp() {
	mockTemperatureDependentSchedulerRepository = std::make_shared<StrictMock<MockTemperatureDependentSchedulerRepository>>();
	mockTemperatureForecast = std::make_shared<StrictMock<MockTemperatureForecast>>();
	mockTemperatureHistory = std::make_shared<StrictMock<MockTemperatureHistory>>();

	scheduler = std::make_shared<TemperatureDependentSchedulerImpl>(
			std::make_shared<TemperatureDependentSchedulerImpl::PersistedData>(
				mockTemperatureDependentSchedulerRepository, id
			),
			mockTemperatureForecast,
			mockTemperatureHistory
		);

	scheduler->setRemainingCorrection(1.0f);
	scheduler->setMinAdjustment(0);
	scheduler->setMaxAdjustment(0);

	TemperatureToPercent::getInstance().setTemperatureAndPercents(std::vector<std::pair<float, unsigned>>{
		{ 15.0f, 25U },
		{ 25.0f, 50U },
		{ 35.0f, 100U }
	});
}

void TemperatureDependentSchedulerImplTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(TemperatureDependentSchedulerImplTest, getAdjustment) {
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(LocalDateTime(2019, 8, 1, 4, 0, 0), LocalDateTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 25)));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 1, 4, 0, 0)), Pointee(Scheduler::Result(50u)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(50));
}

TEST_F(TemperatureDependentSchedulerImplTest, getAdjustmentLower) {
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(LocalDateTime(2019, 8, 1, 4, 0, 0), LocalDateTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 9)));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 1, 4, 0, 0)), Pointee(Scheduler::Result(0u)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(0));
}

TEST_F(TemperatureDependentSchedulerImplTest, getAdjustmentHigher) {
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(LocalDateTime(2019, 8, 1, 4, 0, 0), LocalDateTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 36)));

	EXPECT_THAT(scheduler->process(LocalDateTime(2019, 8, 1, 4, 0, 0)), Pointee(Scheduler::Result(105u)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(105));
}

TEST_F(TemperatureDependentSchedulerImplTest, getRequiredPercentForNextDay) {
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(LocalDateTime(2019, 8, 1, 4, 0, 0), LocalDateTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 25)));

	EXPECT_THAT(scheduler->getRequiredPercentForNextDay(LocalDateTime(2019, 8, 1, 4, 0, 0)), Eq(50));
}

TEST_F(TemperatureDependentSchedulerImplTest, getRequiredPercentForPreviousDay) {
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 1, 4, 0, 0), LocalDateTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 25, 0)));

	EXPECT_THAT(scheduler->getRequiredPercentForPreviousDay(LocalDateTime(2019, 8, 2, 4, 0, 0)), Eq(50));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(TemperatureDependentSchedulerImplTest, getAdjustmentToday) {
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(LocalDateTime(2020, 2, 15, 4, 0, 0), LocalDateTime(2020, 2, 16, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 25)));

	EXPECT_THAT(scheduler->process(LocalDateTime(2020, 2, 15, 4, 0, 0)), Pointee(Scheduler::Result(50u)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(50));

	EXPECT_THAT(scheduler->process(LocalDateTime(2020, 2, 15, 14, 0, 0)), Pointee(Scheduler::Result(0u)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(50));
}

TEST_F(TemperatureDependentSchedulerImplTest, getAdjustmentYesterdayDay) {
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(LocalDateTime(2020, 2, 15, 4, 0, 0), LocalDateTime(2020, 2, 16, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 25)));
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2020, 2, 15, 4, 0, 0), LocalDateTime(2020, 2, 16, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 25, 0)));
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(LocalDateTime(2020, 2, 16, 4, 0, 0), LocalDateTime(2020, 2, 17, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 30)));

	EXPECT_THAT(scheduler->process(LocalDateTime(2020, 2, 15, 4, 0, 0)), Pointee(Scheduler::Result(50u)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(50));

	EXPECT_THAT(scheduler->process(LocalDateTime(2020, 2, 16, 4, 0, 0)), Pointee(Scheduler::Result(75u)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(75));
}

TEST_F(TemperatureDependentSchedulerImplTest, getAdjustmentOtherDay) {
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(LocalDateTime(2020, 2, 15, 4, 0, 0), LocalDateTime(2020, 2, 16, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 25)));
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(LocalDateTime(2020, 2, 17, 4, 0, 0), LocalDateTime(2020, 2, 18, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 30)));

	EXPECT_THAT(scheduler->process(LocalDateTime(2020, 2, 15, 4, 0, 0)), Pointee(Scheduler::Result(50u)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(50));

	EXPECT_THAT(scheduler->process(LocalDateTime(2020, 2, 17, 4, 0, 0)), Pointee(Scheduler::Result(75u)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(75));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(TemperatureDependentSchedulerImplTest, dayStart) {
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(LocalDateTime(2020, 2, 28, 0, 0, 0), LocalDateTime(2020, 2, 28, 23, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 15)));

	EXPECT_THAT(scheduler->process(LocalDateTime(2020, 2, 28, 0, 0, 0)), Pointee(Scheduler::Result(25u)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(25));

	EXPECT_THAT(scheduler->process(LocalDateTime(2020, 2, 28, 23, 59, 59)), Pointee(Scheduler::Result(0u)));
	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(25));
}

///////////////////////////////////////////////////////////////////////////////
/*
TEST_F(TemperatureDependentSchedulerImplTest, loadPersistedData) {
	const int expectedRemainingPercent = 98;
	const time_t expectedLastRun = 45;

	const auto entity = std::make_shared<TemperatureDependentSchedulerEntity>(id, expectedRemainingPercent, expectedLastRun);

	EXPECT_CALL(*mockTemperatureDependentSchedulerRepository, findById(id)).
			Times(1).
			WillOnce(Return(entity));

	scheduler->loadPersistedData();

	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(expectedRemainingPercent));
	EXPECT_THAT(scheduler->getLastRun(), Eq(DateTime::create(expectedLastRun)));
}

TEST_F(TemperatureDependentSchedulerImplTest, savePersistedData) {
	const unsigned id = 20;
	const int expectedRemainingPercent = 98;
	const time_t expectedLastRun = 45;

	const auto entity = std::make_shared<TemperatureDependentSchedulerEntity>(id, expectedRemainingPercent, expectedLastRun);

	EXPECT_CALL(*mockTemperatureDependentSchedulerRepository, findById(id)).
			Times(1).
			WillOnce(Return(entity));

	EXPECT_CALL(*mockTemperatureDependentSchedulerRepository, update(Pointee(TemperatureDependentSchedulerEntity(id, expectedRemainingPercent, expectedLastRun)))).
			Times(1);

	scheduler->loadPersistedData();

	EXPECT_THAT(scheduler->getRemainingPercent(), Eq(expectedRemainingPercent));
	EXPECT_THAT(scheduler->getLastRun(), Eq(DateTime::create(expectedLastRun)));

	scheduler->savePersistedData();
}

*/
