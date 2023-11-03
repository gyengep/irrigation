#include <gmock/gmock.h>
#include <memory>
#include "Mocks/MockTemperatureForecast.h"
#include "Mocks/MockTemperatureHistory.h"
#include "Schedulers/Impl/TemperatureDependentSchedulerImpl.h"


class TemperatureDependentSchedulerImplTest : public ::testing::Test {
protected:

	std::shared_ptr<MockTemperatureForecast> mockTemperatureForecast;
	std::shared_ptr<MockTemperatureHistory> mockTemperatureHistory;
	std::shared_ptr<TemperatureDependentSchedulerImpl> temperatureDependentScheduler;

	virtual void SetUp();
    virtual void TearDown();
};

///////////////////////////////////////////////////////////////////////////////

class TemperatureDependentSchedulerImplProcessTest : public TemperatureDependentSchedulerImplTest {
protected:

	virtual void SetUp();
    virtual void TearDown();
};

///////////////////////////////////////////////////////////////////////////////

class TemperatureDependentSchedulerImplDtoTest : public TemperatureDependentSchedulerImplTest {
protected:

	static const float originalRemainingCorrection;
	static const int originalMinAdjustment = 123;
	static const int originalMaxAdjustment = 999;

	static const float newRemainingCorrection;
	static const int newMinAdjustment = 654;
	static const int newMaxAdjustment = 777;

	virtual void SetUp();
    virtual void TearDown();
};
