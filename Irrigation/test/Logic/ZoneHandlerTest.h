#pragma once
#include <gmock/gmock.h>
#include <vector>
#include "Hardware/Valves.h"
#include "Logic/ZoneHandler.h"



class MockValves : public Valves {
public:
	MOCK_METHOD0(resetAll, void());
	MOCK_METHOD2(activate, void(size_t valveID, bool active));
	MOCK_METHOD2(activate, void(const std::vector<size_t>& valveIDs, bool active));
};


class ZoneHandlerTestWithMock : public ::testing::Test {
protected:

	ZoneHandler zones;
	MockValves* mockValves;

    virtual void SetUp();
    virtual void TearDown();
};
