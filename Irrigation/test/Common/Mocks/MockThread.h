#include <gmock/gmock.h>
#include "Utils/Thread.h"


class MockThread : public Thread {
public:
	MOCK_METHOD0(onExecute, void());
};
