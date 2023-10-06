#pragma once
#include <gmock/gmock.h>
#include "Persistence/TemperatureDependentSchedulerRepository.h"


class MockTemperatureDependentSchedulerRepository : public TemperatureDependentSchedulerRepository {
public:

	MOCK_METHOD0(init, void());
	MOCK_METHOD0(uninit, void());

	MOCK_CONST_METHOD0(findAll, std::list<TemperatureDependentSchedulerEntityPtr>());
	MOCK_CONST_METHOD1(findById, const TemperatureDependentSchedulerEntityPtr(unsigned id));

	MOCK_METHOD1(insert, void(const TemperatureDependentSchedulerEntityPtr& entity));
	MOCK_METHOD1(erase, void(const TemperatureDependentSchedulerEntityPtr& entity));
	MOCK_METHOD1(update, void(const TemperatureDependentSchedulerEntityPtr& entity));

	MOCK_CONST_METHOD0(toString, std::string());
};
