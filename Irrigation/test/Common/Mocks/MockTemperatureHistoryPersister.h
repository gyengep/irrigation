#pragma once
#include <gmock/gmock.h>
#include "Temperature/TemperatureHistoryPersister.h"


class MockTemperatureHistoryPersister : public TemperatureHistoryPersister {
public:
	MOCK_METHOD1(removeOlder, void(const DateTime& dateTime));
	MOCK_METHOD1(removeNewer, void(const DateTime& dateTime));
	MOCK_METHOD1(add, void(const Sample& sample));
	MOCK_CONST_METHOD2(getBetween, std::list<TemperatureHistoryPersister::Sample>(const DateTime& from, const DateTime& to));
	MOCK_CONST_METHOD0(getAll, std::list<TemperatureHistoryPersister::Sample>());
};
