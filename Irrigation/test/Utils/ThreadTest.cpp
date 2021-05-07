#include <gmock/gmock.h>
#include "Mocks/MockRunnable.h"
#include "Mocks/MockThread.h"
#include "Exceptions/InterruptedException.h"
#include "Utils/Runnable.h"
#include "Utils/Thread.h"

using namespace std;
using namespace testing;


TEST(ThreadTest, threadNotStarted) {
	auto mockRunnable = make_shared<MockRunnable>();
	Thread thread(mockRunnable, "");

	EXPECT_CALL(*mockRunnable, run()).Times(0);
	EXPECT_CALL(*mockRunnable, interrupt()).Times(0);
}

TEST(ThreadTest, onStarted) {
	auto mockRunnable = make_shared<MockRunnable>();
	Thread thread(mockRunnable, "");

	EXPECT_CALL(*mockRunnable, run()).Times(1);
	EXPECT_CALL(*mockRunnable, interrupt()).Times(1);

	thread.start();
	thread.stop();
}

TEST(ThreadTest, stopWithoutStart) {
	auto mockRunnable = make_shared<MockRunnable>();
	Thread thread(mockRunnable, "");

	EXPECT_CALL(*mockRunnable, run()).Times(0);
	EXPECT_CALL(*mockRunnable, interrupt()).Times(0);

	EXPECT_THROW(thread.stop(), logic_error);
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

	auto mockRunnable = make_shared<MockRunnable>();
	Thread thread(mockRunnable, "");

	EXPECT_CALL(*mockRunnable, run()).Times(1).WillOnce(Invoke(wait));
	EXPECT_CALL(*mockRunnable, interrupt()).Times(1);

	status = Status::Started;
	thread.start();
	thread.stop();
	EXPECT_THAT(status, Eq(Status::Finished));
}

TEST(ThreadTest, threadStopsWorkingOnException) {
	auto mockRunnable = make_shared<MockRunnable>();
	MockThread mockThread(mockRunnable);

	EXPECT_CALL(*mockRunnable, run()).Times(1).WillOnce(Throw(std::runtime_error("")));
	EXPECT_CALL(*mockRunnable, interrupt()).Times(1);

	mockThread.start();
	mockThread.stop();
}

TEST(ThreadTest, threadStopsWorkingOnInterruptedException) {
	auto mockRunnable = make_shared<MockRunnable>();
	MockThread mockThread(mockRunnable);

	EXPECT_CALL(*mockRunnable, run()).Times(1).WillOnce(Throw(InterruptedException("")));
	EXPECT_CALL(*mockRunnable, interrupt()).Times(1);

	mockThread.start();
	mockThread.stop();
}
