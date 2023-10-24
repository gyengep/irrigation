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

class TemperatureDependentSchedulerImplFromDtoTest : public TemperatureDependentSchedulerImplTest {
protected:

	enum class UpdateType {
		Nothing,
		RemainingCorrection,
		MinAdjustment,
		MaxAdjustment,
		All
	};

   	void checkUpdateFromTemperatureDependentSchedulerDto(const UpdateType updateType);
};
