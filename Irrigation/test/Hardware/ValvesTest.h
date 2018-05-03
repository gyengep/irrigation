#pragma once
#include <gmock/gmock.h>
#include "Hardware/Valves.h"



class ValvesTestWithMock : public ::testing::Test {
protected:

	class MockValves : public Valves {
	public:
		MOCK_METHOD2(setPin, void(int, int));
	};


	MockValves* mockValves;

    virtual void SetUp();
    virtual void TearDown();
};
