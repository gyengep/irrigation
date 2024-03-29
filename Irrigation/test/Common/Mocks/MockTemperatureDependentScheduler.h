#pragma once
#include <gmock/gmock.h>
#include "Schedulers/TemperatureDependentScheduler.h"


class MockTemperatureDependentScheduler : public TemperatureDependentScheduler {
public:

	MOCK_METHOD1(process, std::unique_ptr<Scheduler::Result>(const LocalDateTime& localDateTime));

	MOCK_METHOD1(setRemainingCorrection, void(float a));
	MOCK_METHOD1(setMinAdjustment, void(unsigned minAdjustment));
	MOCK_METHOD1(setMaxAdjustment, void(unsigned maxAdjustment));
	MOCK_METHOD1(trimAdjustmentOver, void(unsigned percent));

	MOCK_CONST_METHOD0(toTemperatureDependentSchedulerDto, TemperatureDependentSchedulerDto());
	MOCK_METHOD1(updateFromTemperatureDependentSchedulerDto, void(const TemperatureDependentSchedulerDto& schedulerDto));

	MOCK_CONST_METHOD0(toString, std::string());

	MOCK_CONST_METHOD0(saveTo, nlohmann::json());
	MOCK_METHOD1(loadFrom, void(const nlohmann::json& json));
};

///////////////////////////////////////////////////////////////////////////////

class MockTemperatureDependentSchedulerFactory : public TemperatureDependentSchedulerFactory {
public:
	MOCK_CONST_METHOD0(create, TemperatureDependentSchedulerPtr());
};

