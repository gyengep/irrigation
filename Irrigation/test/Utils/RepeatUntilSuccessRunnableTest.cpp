#include "RepeatUntilSuccessRunnableTest.h"
#include "Utils/WaitObserverStore.h"
#include "TestCommon/cout.h"
#include <stdexcept>
#include <thread>

using namespace testing;


void RepeatUntilSuccessRunnableTest::SetUp() {
	mockWaitObserver = std::make_shared<MockWaitObserver>();
	mockRunnable = std::make_shared<MockRunnable>();

	waitTimes = std::vector<std::chrono::milliseconds> {
		std::chrono::milliseconds(100),
		std::chrono::milliseconds(200),
		std::chrono::milliseconds(300)
	};

	WaitObserverStore::getInstance().insert("TestRepeatUntilSuccessRunnableSync", mockWaitObserver);

	repeatUntilSuccessRunnable = std::unique_ptr<RepeatUntilSuccessRunnable>(
		new RepeatUntilSuccessRunnable(
			mockRunnable,
			waitTimes,
			"Test runnable"
	));

	repeatUntilSuccessRunnable->setSynchronizationObjectName("TestRepeatUntilSuccessRunnableSync");
}

void RepeatUntilSuccessRunnableTest::TearDown() {
	WaitObserverStore::getInstance().clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RepeatUntilSuccessRunnableTest, runSuccess) {
	EXPECT_CALL(*mockRunnable, run()).Times(1);
	EXPECT_CALL(*mockRunnable, interrupt()).Times(0);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(_)).Times(0);

	EXPECT_NO_THROW(repeatUntilSuccessRunnable->run());
}

TEST_F(RepeatUntilSuccessRunnableTest, runFailedThanSuccess) {
	EXPECT_CALL(*mockRunnable, run()).
			Times(5).
			WillOnce(Throw(std::runtime_error(""))).
			WillOnce(Throw(std::runtime_error(""))).
			WillOnce(Throw(std::runtime_error(""))).
			WillOnce(Throw(std::runtime_error(""))).
			WillOnce(Return());

	EXPECT_CALL(*mockRunnable, interrupt()).Times(0);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(waitTimes[0])).Times(1);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(waitTimes[1])).Times(1);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(waitTimes[2])).Times(2);

	EXPECT_NO_THROW(repeatUntilSuccessRunnable->run());
}

TEST_F(RepeatUntilSuccessRunnableTest, runFailedThanInterrupt_TIMING) {
	EXPECT_CALL(*mockRunnable, run()).
			Times(AtLeast(2)).
			WillRepeatedly(Throw(std::runtime_error("")));

	EXPECT_CALL(*mockRunnable, interrupt()).Times(1);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(waitTimes[0])).Times(1);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(waitTimes[1])).Times(1);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(waitTimes[2])).Times(AnyNumber());

	auto wait500ThanInterrupt = [this] () {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		repeatUntilSuccessRunnable->interrupt();
	};

	std::thread thread(wait500ThanInterrupt);

	EXPECT_THROW(repeatUntilSuccessRunnable->run(), InterruptedException);
	thread.join();
}

TEST_F(RepeatUntilSuccessRunnableTest, interruptDuringRunSuccess_TIMING) {
	auto wait500 = [this] () {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	};

	EXPECT_CALL(*mockRunnable, run()).
			Times(1).
			WillRepeatedly(DoAll(InvokeWithoutArgs(wait500), Return()));

	EXPECT_CALL(*mockRunnable, interrupt()).Times(1);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(_)).Times(0);

	auto wait100ThanInterrupt = [this] () {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		repeatUntilSuccessRunnable->interrupt();
	};

	std::thread thread(wait100ThanInterrupt);

	EXPECT_NO_THROW(repeatUntilSuccessRunnable->run());
	thread.join();
}

TEST_F(RepeatUntilSuccessRunnableTest, interruptDuringRunFailed_TIMING) {
	auto wait500 = [this] () {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	};

	EXPECT_CALL(*mockRunnable, run()).
			Times(1).
			WillRepeatedly(DoAll(InvokeWithoutArgs(wait500), Throw(std::runtime_error(""))));

	EXPECT_CALL(*mockRunnable, interrupt()).Times(1);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(waitTimes[0])).Times(1);

	auto wait100ThanInterrupt = [this] () {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		repeatUntilSuccessRunnable->interrupt();
	};

	std::thread thread(wait100ThanInterrupt);

	EXPECT_THROW(repeatUntilSuccessRunnable->run(), InterruptedException);
	thread.join();
}

TEST_F(RepeatUntilSuccessRunnableTest, interruptDuringWait_TIMING) {
	EXPECT_CALL(*mockRunnable, run()).
			Times(1).
			WillRepeatedly(Throw(std::runtime_error("")));

	EXPECT_CALL(*mockRunnable, interrupt()).Times(1);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(waitTimes[0])).Times(1);

	auto wait10ThanInterrupt = [this] () {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		repeatUntilSuccessRunnable->interrupt();
	};

	std::thread thread(wait10ThanInterrupt);

	EXPECT_THROW(repeatUntilSuccessRunnable->run(), InterruptedException);
	thread.join();
}
