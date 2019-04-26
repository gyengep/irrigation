#pragma once
#include <gmock/gmock.h>
#include "Logic/RunTimeContainer.h"


class MockRunTimeContainer : public RunTimeContainer {
public:
	MOCK_METHOD1(updateFromRunTimeDtoList, void(const std::list<RunTimeDTO>& runTimeDtoList));
};
