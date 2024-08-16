#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "Schedulers/Impl/HotWeatherSchedulerImpl.h"
#include "Mocks/MockTemperatureHistory.h"


class HotWeatherSchedulerImplTest : public ::testing::Test {
protected:

	std::shared_ptr<HotWeatherSchedulerImpl> hotWeatherScheduler;
	std::shared_ptr<MockTemperatureHistory> mockTemperatureHistory;

    virtual void SetUp();
    virtual void TearDown();

    void checkDay(int year, int month, int day, bool expectedResult);
};

///////////////////////////////////////////////////////////////////////////////

class HotWeatherSchedulerImplProcessTest : public HotWeatherSchedulerImplTest {
protected:
};

///////////////////////////////////////////////////////////////////////////////

class HotWeatherSchedulerImplFromDtoTest : public HotWeatherSchedulerImplTest {
protected:

	enum class UpdateType {
		Nothing,
		Period,
		MinTemperature,
		All
	};

   	void checkUpdateFromHotWeatherSchedulerDto(const UpdateType updateType);
};
