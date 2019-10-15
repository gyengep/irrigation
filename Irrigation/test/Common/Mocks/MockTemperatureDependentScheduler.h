#pragma once
#include <gmock/gmock.h>
#include "Schedulers/TemperatureDependentScheduler.h"


class MockTemperatureDependentScheduler : public TemperatureDependentScheduler {
public:
	MockTemperatureDependentScheduler() : TemperatureDependentScheduler(nullptr, nullptr) {}
	MOCK_METHOD1(updateFromTemperatureDependentSchedulerDto, void(const TemperatureDependentSchedulerDTO&));
};

