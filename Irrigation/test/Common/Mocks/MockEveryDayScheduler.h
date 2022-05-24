#pragma once
#include <gmock/gmock.h>
#include "Schedulers/EveryDayScheduler.h"


class MockEveryDayScheduler : public EveryDayScheduler {
public:

	MOCK_METHOD1(process, Scheduler::Result(const std::time_t rawtime));

	MOCK_CONST_METHOD0(toEveryDaySchedulerDto, EveryDaySchedulerDTO());
	MOCK_METHOD1(updateFromEveryDaySchedulerDto, void(const EveryDaySchedulerDTO& schedulerDTO));

	MOCK_CONST_METHOD0(toString, std::string());
};

///////////////////////////////////////////////////////////////////////////////

class MockEveryDaySchedulerFactory : public EveryDaySchedulerFactory {
public:
	virtual ~MockEveryDaySchedulerFactory() = default;
	virtual EveryDaySchedulerPtr create() const override {
		return std::make_shared<MockEveryDayScheduler>();
	}
};
