#include <chrono>
#include <thread>
#include "FixedDelaySchedulerThreadTest.h"
#include "Mocks/MockRunnable.h"
#include "Utils/Runnable.h"
#include "Utils/WaitObserverStore.h"

using namespace std;
using namespace testing;


void FixedDelaySchedulerThreadTest::SetUp() {
	mockWaitObserver = std::make_shared<MockWaitObserver>();
	mockRunnable = std::make_shared<MockRunnable>();

	WaitObserverStore::getInstance().insert("TestFixedDelaySchedulerThreadSync", mockWaitObserver);
}

void FixedDelaySchedulerThreadTest::TearDown() {
	WaitObserverStore::getInstance().clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(FixedDelaySchedulerThreadTest, schedulerThreadNotStarted) {
	FixedDelaySchedulerThread fixedDelaySchedulerThread(
			mockRunnable,
			std::chrono::milliseconds(100),
			std::chrono::milliseconds(100),
			""
	);

	fixedDelaySchedulerThread.setSynchronizationObjectName("TestFixedDelaySchedulerThreadSync");

	EXPECT_CALL(*mockRunnable, run()).Times(0);
	EXPECT_CALL(*mockRunnable, interrupt()).Times(0);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(_)).Times(0);
}

TEST_F(FixedDelaySchedulerThreadTest, run_0_times_TIMING) {
	FixedDelaySchedulerThread fixedDelaySchedulerThread(
			mockRunnable,
			std::chrono::milliseconds(500),
			std::chrono::milliseconds(1000),
			""
	);

	fixedDelaySchedulerThread.setSynchronizationObjectName("TestFixedDelaySchedulerThreadSync");

	EXPECT_CALL(*mockRunnable, run()).Times(0);
	EXPECT_CALL(*mockRunnable, interrupt()).Times(1);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(std::chrono::milliseconds(500))).Times(1);

	fixedDelaySchedulerThread.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	fixedDelaySchedulerThread.stop();
}

TEST_F(FixedDelaySchedulerThreadTest, run_1_times_TIMING) {
	FixedDelaySchedulerThread fixedDelaySchedulerThread(
			mockRunnable,
			std::chrono::milliseconds(500),
			std::chrono::milliseconds(1000),
			""
	);

	fixedDelaySchedulerThread.setSynchronizationObjectName("TestFixedDelaySchedulerThreadSync");

	EXPECT_CALL(*mockRunnable, run()).Times(1);
	EXPECT_CALL(*mockRunnable, interrupt()).Times(1);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(std::chrono::milliseconds(500))).Times(1);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(std::chrono::milliseconds(1000))).Times(1);

	fixedDelaySchedulerThread.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(750));
	fixedDelaySchedulerThread.stop();
}

TEST_F(FixedDelaySchedulerThreadTest, run_more_times_TIMING) {
	FixedDelaySchedulerThread fixedDelaySchedulerThread(
			mockRunnable,
			std::chrono::milliseconds(50),
			std::chrono::milliseconds(100),
			""
	);

	fixedDelaySchedulerThread.setSynchronizationObjectName("TestFixedDelaySchedulerThreadSync");

	EXPECT_CALL(*mockRunnable, run()).Times(AtLeast(3));
	EXPECT_CALL(*mockRunnable, interrupt()).Times(1);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(std::chrono::milliseconds(50))).Times(1);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(std::chrono::milliseconds(100))).Times(AtLeast(3));

	fixedDelaySchedulerThread.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	fixedDelaySchedulerThread.stop();
}

TEST_F(FixedDelaySchedulerThreadTest, threadStopsWorkingOnException_TIMING) {
	FixedDelaySchedulerThread fixedDelaySchedulerThread(
			mockRunnable,
			std::chrono::milliseconds(10),
			std::chrono::milliseconds(10),
			""
	);

	fixedDelaySchedulerThread.setSynchronizationObjectName("TestFixedDelaySchedulerThreadSync");

	EXPECT_CALL(*mockRunnable, run()).Times(1).WillRepeatedly(Throw(std::runtime_error("")));
	EXPECT_CALL(*mockRunnable, interrupt()).Times(1);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(std::chrono::milliseconds(10))).Times(1);

	fixedDelaySchedulerThread.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	fixedDelaySchedulerThread.stop();
}

TEST_F(FixedDelaySchedulerThreadTest, threadStopsWorkingOnInterruptedException_TIMING) {
	FixedDelaySchedulerThread fixedDelaySchedulerThread(
			mockRunnable,
			std::chrono::milliseconds(10),
			std::chrono::milliseconds(10),
			""
	);

	fixedDelaySchedulerThread.setSynchronizationObjectName("TestFixedDelaySchedulerThreadSync");

	EXPECT_CALL(*mockRunnable, run()).Times(1).WillRepeatedly(Throw(InterruptedException("")));
	EXPECT_CALL(*mockRunnable, interrupt()).Times(1);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(std::chrono::milliseconds(10))).Times(1);

	fixedDelaySchedulerThread.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	fixedDelaySchedulerThread.stop();
}
