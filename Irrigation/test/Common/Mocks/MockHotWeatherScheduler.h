#pragma once
#include <gmock/gmock.h>
#include "Schedulers/HotWeatherScheduler.h"


class MockHotWeatherScheduler : public HotWeatherScheduler {
public:

	MOCK_METHOD1(process, Scheduler::Result(const std::time_t rawtime));

	MOCK_METHOD1(setMinTemperature, void(float minTemperature));
	MOCK_METHOD1(setPeriod, void(const std::chrono::seconds& period));

	MOCK_CONST_METHOD0(toHotWeatherSchedulerDto, HotWeatherSchedulerDTO());
	MOCK_METHOD1(updateFromHotWeatherSchedulerDto, void(const HotWeatherSchedulerDTO& schedulerDTO));

	MOCK_CONST_METHOD0(toString, std::string());

	MOCK_CONST_METHOD0(saveTo, nlohmann::json());
	MOCK_METHOD1(loadFrom, void(const nlohmann::json& json));
};

///////////////////////////////////////////////////////////////////////////////

class MockHotWeatherSchedulerFactory : public HotWeatherSchedulerFactory {
public:
	virtual ~MockHotWeatherSchedulerFactory() = default;
	virtual HotWeatherSchedulerPtr create() const override {
		return std::make_shared<MockHotWeatherScheduler>();
	}
};
