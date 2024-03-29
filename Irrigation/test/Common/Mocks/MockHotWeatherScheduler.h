#pragma once
#include <gmock/gmock.h>
#include "Schedulers/HotWeatherScheduler.h"


class MockHotWeatherScheduler : public HotWeatherScheduler {
public:

	MOCK_METHOD1(process, std::unique_ptr<Scheduler::Result>(const LocalDateTime& localDateTime));

	MOCK_METHOD1(setMinTemperature, void(float minTemperature));
	MOCK_METHOD1(setPeriod, void(const std::chrono::seconds& period));

	MOCK_CONST_METHOD0(toHotWeatherSchedulerDto, HotWeatherSchedulerDto());
	MOCK_METHOD1(updateFromHotWeatherSchedulerDto, void(const HotWeatherSchedulerDto& schedulerDto));

	MOCK_CONST_METHOD0(toString, std::string());

	MOCK_CONST_METHOD0(saveTo, nlohmann::json());
	MOCK_METHOD1(loadFrom, void(const nlohmann::json& json));
};

///////////////////////////////////////////////////////////////////////////////

class MockHotWeatherSchedulerFactory : public HotWeatherSchedulerFactory {
public:
	MOCK_CONST_METHOD0(create, HotWeatherSchedulerPtr());
};
