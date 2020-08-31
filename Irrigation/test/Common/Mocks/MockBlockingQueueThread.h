#include <gmock/gmock.h>
#include "Utils/BlockingQueueThread.h"


class MockBlockingQueueThread : public BlockingQueueThread<int> {
public:
	MockBlockingQueueThread(const std::vector<std::chrono::milliseconds>& waitTimes) : BlockingQueueThread<int>("", waitTimes) {}

	MOCK_METHOD1(onItemAvailable, void(const int& value));
	MOCK_METHOD0(onResumed, void());
	MOCK_METHOD2(onError, void(size_t errorCount, const std::chrono::milliseconds&));
};
