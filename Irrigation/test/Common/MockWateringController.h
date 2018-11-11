#pragma once
#include <gmock/gmock.h>
#include "Logic/WateringController.h"



class MockWateringController : public WateringController {
public:
	MOCK_CONST_METHOD0(isWateringActive, bool());
	MOCK_METHOD1(on1SecTimer, void(const std::time_t&));
	MOCK_METHOD3(start, void(const std::time_t& rawTime, const RunTimeContainer& runTimes, unsigned adjustmentPercent));
	MOCK_METHOD0(stop, void ());
};
