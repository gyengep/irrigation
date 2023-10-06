#pragma once
#include <gmock/gmock.h>
#include "Schedulers/EveryDayScheduler.h"


class MockEveryDayScheduler : public EveryDayScheduler {
public:

	MOCK_METHOD1(process, std::unique_ptr<Scheduler::Result>(const LocalDateTime& localDateTime));

	MOCK_CONST_METHOD0(toEveryDaySchedulerDto, EveryDaySchedulerDTO());
	MOCK_METHOD1(updateFromEveryDaySchedulerDto, void(const EveryDaySchedulerDTO& schedulerDTO));

	MOCK_METHOD0(createPersistedData, void());
	MOCK_METHOD0(deletePersistedData, void());
	MOCK_METHOD0(loadPersistedData, void());

	MOCK_CONST_METHOD0(toString, std::string());
};

///////////////////////////////////////////////////////////////////////////////

class MockEveryDaySchedulerFactory : public EveryDaySchedulerFactory {
public:
	MOCK_CONST_METHOD0(create, EveryDaySchedulerPtr());
};
