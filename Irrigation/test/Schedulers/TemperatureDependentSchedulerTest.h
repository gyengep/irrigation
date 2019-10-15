#include <gmock/gmock.h>
#include <memory>
#include "Mocks/MockTemperatureForecast.h"
#include "Mocks/MockTemperatureHistory.h"
#include "Schedulers/TemperatureDependentScheduler.h"


class TemperatureDependentSchedulerTest : public ::testing::Test {
protected:

	std::shared_ptr<MockTemperatureForecast> mockTemperatureForecast;
	std::shared_ptr<MockTemperatureHistory> mockTemperatureHistory;
	std::unique_ptr<TemperatureDependentScheduler> scheduler;

	virtual void SetUp();
    virtual void TearDown();
};
