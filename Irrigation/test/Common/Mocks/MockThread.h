#pragma once
#include <gmock/gmock.h>
#include "Utils/Thread.h"


class MockThread : public Thread {
public:
	MockThread(const std::shared_ptr<Runnable>& runnable) : Thread(runnable, "") {}
	MockThread() : Thread("") {}

	MOCK_METHOD0(run, void());
	MOCK_METHOD0(interrupt, void());
};
