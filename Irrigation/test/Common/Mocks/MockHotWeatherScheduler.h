#pragma once
#include <gmock/gmock.h>
#include "Schedulers/HotWeatherScheduler.h"


class MockHotWeatherScheduler : public HotWeatherScheduler {
public:
	MockHotWeatherScheduler() : HotWeatherScheduler(nullptr) {}
	MOCK_METHOD1(updateFromHotWeatherSchedulerDto, void(const HotWeatherSchedulerDTO&));
};

