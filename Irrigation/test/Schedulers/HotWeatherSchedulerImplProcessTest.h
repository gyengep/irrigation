#include <gmock/gmock.h>
#include <memory>
#include "Mocks/MockTemperatureHistory.h"
#include "Schedulers/HotWeatherSchedulerImpl.h"


class HotWeatherSchedulerImplProcessTest : public ::testing::Test {
protected:

	std::shared_ptr<MockTemperatureHistory> mockTemperatureHistory;
	std::shared_ptr<HotWeatherSchedulerImpl> scheduler;

	virtual void SetUp();
    virtual void TearDown();
};
