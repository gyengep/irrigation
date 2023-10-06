#include <gmock/gmock.h>
#include <memory>
#include "Mocks/MockTemperatureDependentSchedulerRepository.h"
#include "Mocks/MockTemperatureForecast.h"
#include "Mocks/MockTemperatureHistory.h"
#include "Schedulers/TemperatureDependentSchedulerImpl.h"


class TemperatureDependentSchedulerImplTest : public ::testing::Test {
protected:

	const unsigned id = 20;

	std::shared_ptr<MockTemperatureDependentSchedulerRepository> mockTemperatureDependentSchedulerRepository;
	std::shared_ptr<MockTemperatureForecast> mockTemperatureForecast;
	std::shared_ptr<MockTemperatureHistory> mockTemperatureHistory;

	std::shared_ptr<TemperatureDependentSchedulerImpl> scheduler;

	virtual void SetUp();
    virtual void TearDown();
};
