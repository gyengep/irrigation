#pragma once
#include <gmock/gmock.h>
#include "Model/Application.h"


class MockApplication : public Application {
public:
	MOCK_METHOD0(onInitialize, void());
	MOCK_METHOD0(onTerminate, void());
};
