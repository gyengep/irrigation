#pragma once
#include <gmock/gmock.h>
#include "Temperature/TemperatureHistoryPersister.h"


class MockTemperatureHistoryPersister : public TemperatureHistoryPersister {
public:
	MOCK_METHOD1(removeOlder, void(const std::time_t& rawTime));
	MOCK_METHOD1(removeNewer, void(const std::time_t& rawTime));
	MOCK_METHOD1(add, void(const Sample& sample));
	MOCK_CONST_METHOD2(getBetween, std::list<TemperatureHistoryPersister::Sample>(const std::time_t& from, const std::time_t& to));
	MOCK_CONST_METHOD0(getAll, std::list<TemperatureHistoryPersister::Sample>());
};
