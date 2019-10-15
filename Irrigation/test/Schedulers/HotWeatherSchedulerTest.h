#include <gmock/gmock.h>
#include <memory>
#include "Mocks/MockTemperatureHistory.h"
#include "Schedulers/HotWeatherScheduler.h"


class HotWeatherSchedulerTest : public ::testing::Test {
protected:

	std::shared_ptr<MockTemperatureHistory> mockTemperatureHistory;
	std::unique_ptr<HotWeatherScheduler> scheduler;

	virtual void SetUp();
    virtual void TearDown();
};
