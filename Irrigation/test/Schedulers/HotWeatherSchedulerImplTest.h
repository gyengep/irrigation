#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "schedulers/Impl/HotWeatherSchedulerImpl.h"
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

	static const unsigned originalHours = 15;
	static const unsigned originalMinutes = 25;

	static const unsigned newHours = 16;
	static const unsigned newMinutes = 46;
};

///////////////////////////////////////////////////////////////////////////////

class HotWeatherSchedulerImplDtoTest : public HotWeatherSchedulerImplTest {
protected:

	static const float originalMinTemperature;
	static const unsigned originalPeriodInSeconds = 25;

	static const float newMinTemperature;
	static const unsigned newPeriodInSeconds = 36;

    virtual void SetUp();
    virtual void TearDown();
};
