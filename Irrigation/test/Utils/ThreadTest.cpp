#include <gmock/gmock.h>
#include "Mocks/MockThread.h"
#include "Utils/Thread.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(ThreadTest, threadNotStarted) {
	MockThread mockThread;
	EXPECT_CALL(mockThread, onExecute()).Times(0);
}

TEST(ThreadTest, onStarted) {
	MockThread mockThread;
	EXPECT_CALL(mockThread, onExecute()).Times(1);

	mockThread.start();
	mockThread.join();
}

TEST(ThreadTest, joinWithoutStart) {
	MockThread mockThread;
	EXPECT_THROW(mockThread.join(), logic_error);
}

TEST(ThreadTest, onExecuteCalledWithDelay) {
	enum class Status {
		Init,
		Started,
		Finished
	};

	Status status = Status::Init;

	auto wait = [&status]() {
		EXPECT_THAT(status, Eq(Status::Started));
		std::this_thread::sleep_for(chrono::milliseconds(100));
		status = Status::Finished;
	};

	MockThread mockThread;
	EXPECT_CALL(mockThread, onExecute()).Times(1).WillRepeatedly(Invoke(wait));

	status = Status::Started;
	mockThread.start();
	mockThread.join();
	EXPECT_THAT(status, Eq(Status::Finished));
}
