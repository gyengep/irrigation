#pragma once
#include <gmock/gmock.h>
#include "Utils/Runnable.h"


class MockRunnable : public Runnable {
public:
	MOCK_METHOD0(run, void());
	MOCK_METHOD0(interrupt, void());
};
