#include "TemperatureDependentSchedulerImplTest.h"
#include "Schedulers/TemperatureToPercent.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureDependentSchedulerImplConstructorTest, defaultConstructor) {
	TemperatureDependentSchedulerImpl temperatureDependentScheduler(
			std::make_shared<StrictMock<MockTemperatureForecast>>(),
			std::make_shared<StrictMock<MockTemperatureHistory>>()
		);

	EXPECT_THAT(temperatureDependentScheduler.getRemainingCorrection(), Eq(1.0f));
	EXPECT_THAT(temperatureDependentScheduler.getMinAdjustment(), Eq(0));
	EXPECT_THAT(temperatureDependentScheduler.getMaxAdjustment(), Eq(0));
}

TEST(TemperatureDependentSchedulerImplConstructorTest, initializerConstructor) {
	const float expectedRemainingCorrection = 1.5f;
	const unsigned expectedMinAdjustment = 83;
	const unsigned expectedMaxAdjustment = 147;

	TemperatureDependentSchedulerImpl temperatureDependentScheduler(
			std::make_shared<StrictMock<MockTemperatureForecast>>(),
			std::make_shared<StrictMock<MockTemperatureHistory>>(),
			expectedRemainingCorrection, expectedMinAdjustment, expectedMaxAdjustment
		);

	EXPECT_THAT(temperatureDependentScheduler.getRemainingCorrection(), Eq(expectedRemainingCorrection));
	EXPECT_THAT(temperatureDependentScheduler.getMinAdjustment(), Eq(expectedMinAdjustment));
	EXPECT_THAT(temperatureDependentScheduler.getMaxAdjustment(), Eq(expectedMaxAdjustment));
}

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureDependentSchedulerImplFactoryTest, create) {
	auto mockTemperatureForecast = std::make_shared<NiceMock<MockTemperatureForecast>>();
	auto mockTemperatureHistory = std::make_shared<NiceMock<MockTemperatureHistory>>();

	TemperatureDependentSchedulerImplFactory temperatureDependentSchedulerImplFactory(
			mockTemperatureForecast,
			mockTemperatureHistory
		);

	EXPECT_THAT(temperatureDependentSchedulerImplFactory.create(), Not(IsNull()));
	EXPECT_TRUE(temperatureDependentSchedulerImplFactory.create() != temperatureDependentSchedulerImplFactory.create());
}

///////////////////////////////////////////////////////////////////////////////

void TemperatureDependentSchedulerImplTest::SetUp() {
	mockTemperatureForecast = std::make_shared<StrictMock<MockTemperatureForecast>>();
	mockTemperatureHistory = std::make_shared<StrictMock<MockTemperatureHistory>>();

	temperatureDependentScheduler = std::make_shared<TemperatureDependentSchedulerImpl>(
			mockTemperatureForecast,
			mockTemperatureHistory
		);

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

	EXPECT_THAT(temperatureDependentScheduler->process(LocalDateTime(2019, 8, 1, 4, 0, 0)), Pointee(Scheduler::Result(50u)));
	EXPECT_THAT(temperatureDependentScheduler->getRemainingPercent(), Eq(50));
}

TEST_F(TemperatureDependentSchedulerImplTest, getAdjustmentLower) {
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(LocalDateTime(2019, 8, 1, 4, 0, 0), LocalDateTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 9)));

	EXPECT_THAT(temperatureDependentScheduler->process(LocalDateTime(2019, 8, 1, 4, 0, 0)), Pointee(Scheduler::Result(0u)));
	EXPECT_THAT(temperatureDependentScheduler->getRemainingPercent(), Eq(0));
}

TEST_F(TemperatureDependentSchedulerImplTest, getAdjustmentHigher) {
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(LocalDateTime(2019, 8, 1, 4, 0, 0), LocalDateTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 36)));

	EXPECT_THAT(temperatureDependentScheduler->process(LocalDateTime(2019, 8, 1, 4, 0, 0)), Pointee(Scheduler::Result(105u)));
	EXPECT_THAT(temperatureDependentScheduler->getRemainingPercent(), Eq(105));
}

TEST_F(TemperatureDependentSchedulerImplTest, getRequiredPercentForNextDay) {
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(LocalDateTime(2019, 8, 1, 4, 0, 0), LocalDateTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 25)));

	EXPECT_THAT(temperatureDependentScheduler->getRequiredPercentForNextDay(LocalDateTime(2019, 8, 1, 4, 0, 0)), Eq(50));
}

TEST_F(TemperatureDependentSchedulerImplTest, getRequiredPercentForPreviousDay) {
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2019, 8, 1, 4, 0, 0), LocalDateTime(2019, 8, 2, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 25, 0)));

	EXPECT_THAT(temperatureDependentScheduler->getRequiredPercentForPreviousDay(LocalDateTime(2019, 8, 2, 4, 0, 0)), Eq(50));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(TemperatureDependentSchedulerImplTest, getAdjustmentToday) {
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(LocalDateTime(2020, 2, 15, 4, 0, 0), LocalDateTime(2020, 2, 16, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 25)));

	EXPECT_THAT(temperatureDependentScheduler->process(LocalDateTime(2020, 2, 15, 4, 0, 0)), Pointee(Scheduler::Result(50u)));
	EXPECT_THAT(temperatureDependentScheduler->getRemainingPercent(), Eq(50));

	EXPECT_THAT(temperatureDependentScheduler->process(LocalDateTime(2020, 2, 15, 14, 0, 0)), Pointee(Scheduler::Result(0u)));
	EXPECT_THAT(temperatureDependentScheduler->getRemainingPercent(), Eq(50));
}

TEST_F(TemperatureDependentSchedulerImplTest, getAdjustmentYesterdayDay) {
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(LocalDateTime(2020, 2, 15, 4, 0, 0), LocalDateTime(2020, 2, 16, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 25)));
	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(LocalDateTime(2020, 2, 15, 4, 0, 0), LocalDateTime(2020, 2, 16, 3, 59, 59))).
		WillOnce(Return(TemperatureHistory::Values(0, 25, 0)));
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(LocalDateTime(2020, 2, 16, 4, 0, 0), LocalDateTime(2020, 2, 17, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 30)));

	EXPECT_THAT(temperatureDependentScheduler->process(LocalDateTime(2020, 2, 15, 4, 0, 0)), Pointee(Scheduler::Result(50u)));
	EXPECT_THAT(temperatureDependentScheduler->getRemainingPercent(), Eq(50));

	EXPECT_THAT(temperatureDependentScheduler->process(LocalDateTime(2020, 2, 16, 4, 0, 0)), Pointee(Scheduler::Result(75u)));
	EXPECT_THAT(temperatureDependentScheduler->getRemainingPercent(), Eq(75));
}

TEST_F(TemperatureDependentSchedulerImplTest, getAdjustmentOtherDay) {
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(LocalDateTime(2020, 2, 15, 4, 0, 0), LocalDateTime(2020, 2, 16, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 25)));
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(LocalDateTime(2020, 2, 17, 4, 0, 0), LocalDateTime(2020, 2, 18, 3, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 30)));

	EXPECT_THAT(temperatureDependentScheduler->process(LocalDateTime(2020, 2, 15, 4, 0, 0)), Pointee(Scheduler::Result(50u)));
	EXPECT_THAT(temperatureDependentScheduler->getRemainingPercent(), Eq(50));

	EXPECT_THAT(temperatureDependentScheduler->process(LocalDateTime(2020, 2, 17, 4, 0, 0)), Pointee(Scheduler::Result(75u)));
	EXPECT_THAT(temperatureDependentScheduler->getRemainingPercent(), Eq(75));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(TemperatureDependentSchedulerImplTest, dayStart) {
	EXPECT_CALL(*mockTemperatureForecast, getTemperatureForecast(LocalDateTime(2020, 2, 28, 0, 0, 0), LocalDateTime(2020, 2, 28, 23, 59, 59))).
		WillOnce(Return(TemperatureForecast::Values(0, 15)));

	EXPECT_THAT(temperatureDependentScheduler->process(LocalDateTime(2020, 2, 28, 0, 0, 0)), Pointee(Scheduler::Result(25u)));
	EXPECT_THAT(temperatureDependentScheduler->getRemainingPercent(), Eq(25));

	EXPECT_THAT(temperatureDependentScheduler->process(LocalDateTime(2020, 2, 28, 23, 59, 59)), Pointee(Scheduler::Result(0u)));
	EXPECT_THAT(temperatureDependentScheduler->getRemainingPercent(), Eq(25));
}
