#pragma once
#include <gmock/gmock.h>
#include "Schedulers/Scheduler.h"


class MockScheduler : public Scheduler {
public:
	MOCK_METHOD1(process, Scheduler::Result(const LocalDateTime& localDateTime));
	MOCK_CONST_METHOD0(toString, std::string());
};
