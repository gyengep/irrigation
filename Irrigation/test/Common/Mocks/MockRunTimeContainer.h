#pragma once
#include <gmock/gmock.h>
#include "Logic/RunTimeContainer.h"


class MockRunTimeContainer : public RunTimeContainer {
public:
	MockRunTimeContainer(const std::shared_ptr<RunTimeFactory>& runTimeFactory) : RunTimeContainer(runTimeFactory) {}
	MOCK_METHOD1(updateFromRunTimeDtoList, void(const std::list<RunTimeDTO>& runTimeDtoList));
};
