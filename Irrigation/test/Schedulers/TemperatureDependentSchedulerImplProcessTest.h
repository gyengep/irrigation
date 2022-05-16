#include <gmock/gmock.h>
#include <memory>
#include "Mocks/MockTemperatureForecast.h"
#include "Mocks/MockTemperatureHistory.h"
#include "Schedulers/TemperatureDependentSchedulerImpl.h"


class TemperatureDependentSchedulerImplProcessTest : public ::testing::Test {
protected:

	std::shared_ptr<MockTemperatureForecast> mockTemperatureForecast;
	std::shared_ptr<MockTemperatureHistory> mockTemperatureHistory;
	std::shared_ptr<TemperatureDependentSchedulerImpl> scheduler;

	virtual void SetUp();
    virtual void TearDown();
};
