#pragma once
#include <gmock/gmock.h>
#include "Logic/WateringController.h"


class MockWateringController : public WateringController {
public:
	MOCK_CONST_METHOD0(isWateringActive, bool());
	MOCK_METHOD1(start, void(const DurationList& durations));
	MOCK_METHOD0(stop, void ());
};
