#pragma once
#include <gmock/gmock.h>
#include "Schedulers/WeeklyScheduler.h"


class MockWeeklyScheduler : public WeeklyScheduler {
public:

	MOCK_METHOD1(process, std::unique_ptr<Scheduler::Result>(const LocalDateTime& localDateTime));

	MOCK_METHOD2(enableDay, void(size_t day, bool enable));
	MOCK_CONST_METHOD1(isDayEnabled, bool(size_t day));

	MOCK_CONST_METHOD0(toWeeklySchedulerDto, WeeklySchedulerDTO());
	MOCK_METHOD1(updateFromWeeklySchedulerDto, void(const WeeklySchedulerDTO&));

	MOCK_CONST_METHOD0(toString, std::string());
};

///////////////////////////////////////////////////////////////////////////////

class MockWeeklySchedulerFactory : public WeeklySchedulerFactory {
public:
	MOCK_CONST_METHOD0(create, WeeklySchedulerPtr());
};
