#pragma once
#include <gmock/gmock.h>
#include "Schedulers/HotWeatherScheduler.h"


class MockHotWeatherScheduler : public HotWeatherScheduler {
public:

	MOCK_METHOD1(process, std::unique_ptr<Scheduler::Result>(const LocalDateTime& localDateTime));

	MOCK_METHOD1(setMinTemperature, void(float minTemperature));
	MOCK_METHOD1(setPeriod, void(const std::chrono::seconds& period));

	MOCK_CONST_METHOD0(toHotWeatherSchedulerDto, HotWeatherSchedulerDTO());
	MOCK_METHOD1(updateFromHotWeatherSchedulerDto, void(const HotWeatherSchedulerDTO& schedulerDTO));

	MOCK_METHOD0(createPersistedData, void());
	MOCK_METHOD0(deletePersistedData, void());
	MOCK_METHOD0(loadPersistedData, void());

	MOCK_CONST_METHOD0(toString, std::string());
};

///////////////////////////////////////////////////////////////////////////////

class MockHotWeatherSchedulerFactory : public HotWeatherSchedulerFactory {
public:
	MOCK_CONST_METHOD0(create, HotWeatherSchedulerPtr());
};
