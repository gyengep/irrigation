#pragma once
#include <gmock/gmock.h>
#include "Persistence/HotWeatherSchedulerRepository.h"


class MockHotWeatherSchedulerRepository : public HotWeatherSchedulerRepository {
public:

	MOCK_METHOD0(init, void());
	MOCK_METHOD0(uninit, void());

	MOCK_CONST_METHOD0(findAll, std::list<HotWeatherSchedulerEntityPtr>());
	MOCK_CONST_METHOD1(findById, const HotWeatherSchedulerEntityPtr(unsigned id));

	MOCK_METHOD1(insert, void(const HotWeatherSchedulerEntityPtr& entity));
	MOCK_METHOD1(erase, void(const HotWeatherSchedulerEntityPtr& entity));
	MOCK_METHOD1(update, void(const HotWeatherSchedulerEntityPtr& entity));

	MOCK_CONST_METHOD0(toString, std::string());
};
