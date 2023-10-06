#include "Schedulers/HotWeatherSchedulerImpl.h"
#include "Mocks/MockHotWeatherSchedulerRepository.h"
#include "Mocks/MockTemperatureHistory.h"
#include <gmock/gmock.h>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(HotWeatherSchedulerImplTest, defaultConstructor) {
	HotWeatherSchedulerImpl scheduler(
			nullptr,
			std::make_shared<StrictMock<MockTemperatureHistory>>()
		);

	EXPECT_THAT(scheduler.getMinTemperature(), Eq(35));
	EXPECT_THAT(scheduler.getPeriod(), Eq(2 * 60 * 60));
}

TEST(HotWeatherSchedulerImplTest, parametrizedConstructor) {
	HotWeatherSchedulerImpl scheduler(
			nullptr,
			std::make_shared<StrictMock<MockTemperatureHistory>>(),
			std::chrono::minutes(3),
			15
		);

	EXPECT_THAT(scheduler.getPeriod(), Eq(180));
	EXPECT_THAT(scheduler.getMinTemperature(), Eq(15));
}
/*
TEST(HotWeatherSchedulerImplTest, loadPersistedData) {
	const unsigned id = 20;
	const time_t expectedLastRun = 45;
	const auto mockHotWeatherSchedulerRepository = std::make_shared<StrictMock<MockHotWeatherSchedulerRepository>>();
	const auto entity = std::make_shared<HotWeatherSchedulerEntity>(id, expectedLastRun);

	HotWeatherSchedulerImpl scheduler(
			std::make_shared<HotWeatherSchedulerImpl::PersistedData>(mockHotWeatherSchedulerRepository, id),
			std::make_shared<StrictMock<MockTemperatureHistory>>()
		);

	EXPECT_CALL(*mockHotWeatherSchedulerRepository, findById(id)).
			Times(1).
			WillOnce(Return(entity));

	scheduler.loadPersistedData();

	EXPECT_THAT(scheduler.getLastRun(), Eq(DateTime::create(expectedLastRun)));
}

TEST(HotWeatherSchedulerImplTest, savePersistedData) {
	const unsigned id = 20;
	const time_t expectedLastRun = 45;
	const auto mockHotWeatherSchedulerRepository = std::make_shared<StrictMock<MockHotWeatherSchedulerRepository>>();
	const auto entity = std::make_shared<HotWeatherSchedulerEntity>(id, expectedLastRun);

	HotWeatherSchedulerImpl scheduler(
			std::make_shared<HotWeatherSchedulerImpl::PersistedData>(mockHotWeatherSchedulerRepository, id),
			std::make_shared<StrictMock<MockTemperatureHistory>>()
		);

	EXPECT_CALL(*mockHotWeatherSchedulerRepository, findById(id)).
			Times(1).
			WillOnce(Return(entity));

	EXPECT_CALL(*mockHotWeatherSchedulerRepository, update(Pointee(HotWeatherSchedulerEntity(id, expectedLastRun)))).
			Times(1);

	scheduler.loadPersistedData();

	EXPECT_THAT(scheduler.getLastRun(), Eq(DateTime::create(expectedLastRun)));

	scheduler.savePersistedData();
}
*/
