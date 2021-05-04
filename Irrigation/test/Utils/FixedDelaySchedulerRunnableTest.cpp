#include <chrono>
#include <thread>
#include "FixedDelaySchedulerRunnableTest.h"
#include "Mocks/MockRunnable.h"
#include "Utils/Runnable.h"
#include "Utils/WaitObserverStore.h"

using namespace std;
using namespace testing;


void FixedDelaySchedulerRunnableTest::SetUp() {
	mockWaitObserver = std::make_shared<MockWaitObserver>();
	mockRunnable = std::make_shared<MockRunnable>();

	WaitObserverStore::getInstance().insert("TestFixedDelaySchedulerThreadSync", mockWaitObserver);
}

void FixedDelaySchedulerRunnableTest::TearDown() {
	WaitObserverStore::getInstance().clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(FixedDelaySchedulerRunnableTest, schedulerThreadNotStarted) {
	FixedDelaySchedulerRunnable fixedDelaySchedulerRunnable(
			mockRunnable,
			std::chrono::milliseconds(100),
			std::chrono::milliseconds(100),
			""
	);

	fixedDelaySchedulerRunnable.setSynchronizationObjectName("TestFixedDelaySchedulerThreadSync");

	EXPECT_CALL(*mockRunnable, run()).Times(0);
	EXPECT_CALL(*mockRunnable, interrupt()).Times(0);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(_)).Times(0);
}

TEST_F(FixedDelaySchedulerRunnableTest, run_0_times_TIMING) {
	FixedDelaySchedulerRunnable fixedDelaySchedulerRunnable(
			mockRunnable,
			std::chrono::milliseconds(500),
			std::chrono::milliseconds(1000),
			""
	);

	fixedDelaySchedulerRunnable.setSynchronizationObjectName("TestFixedDelaySchedulerThreadSync");

	EXPECT_CALL(*mockRunnable, run()).Times(0);
	EXPECT_CALL(*mockRunnable, interrupt()).Times(1);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(std::chrono::milliseconds(500))).Times(1);

	std::thread thread([&fixedDelaySchedulerRunnable] {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		fixedDelaySchedulerRunnable.interrupt();
	});

	fixedDelaySchedulerRunnable.run();
	thread.join();
}

TEST_F(FixedDelaySchedulerRunnableTest, run_1_times_TIMING) {
	FixedDelaySchedulerRunnable fixedDelaySchedulerRunnable(
			mockRunnable,
			std::chrono::milliseconds(500),
			std::chrono::milliseconds(1000),
			""
	);

	fixedDelaySchedulerRunnable.setSynchronizationObjectName("TestFixedDelaySchedulerThreadSync");

	EXPECT_CALL(*mockRunnable, run()).Times(1);
	EXPECT_CALL(*mockRunnable, interrupt()).Times(1);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(std::chrono::milliseconds(500))).Times(1);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(std::chrono::milliseconds(1000))).Times(1);

	std::thread thread([&fixedDelaySchedulerRunnable] {
		std::this_thread::sleep_for(std::chrono::milliseconds(750));
		fixedDelaySchedulerRunnable.interrupt();
	});

	fixedDelaySchedulerRunnable.run();
	thread.join();
}

TEST_F(FixedDelaySchedulerRunnableTest, run_more_times_TIMING) {
	FixedDelaySchedulerRunnable fixedDelaySchedulerRunnable(
			mockRunnable,
			std::chrono::milliseconds(50),
			std::chrono::milliseconds(100),
			""
	);

	fixedDelaySchedulerRunnable.setSynchronizationObjectName("TestFixedDelaySchedulerThreadSync");

	EXPECT_CALL(*mockRunnable, run()).Times(AtLeast(3));
	EXPECT_CALL(*mockRunnable, interrupt()).Times(1);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(std::chrono::milliseconds(50))).Times(1);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(std::chrono::milliseconds(100))).Times(AtLeast(3));

	std::thread thread([&fixedDelaySchedulerRunnable] {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		fixedDelaySchedulerRunnable.interrupt();
	});

	fixedDelaySchedulerRunnable.run();
	thread.join();
}

TEST_F(FixedDelaySchedulerRunnableTest, threadStopsWorkingOnException_TIMING) {
	FixedDelaySchedulerRunnable fixedDelaySchedulerRunnable(
			mockRunnable,
			std::chrono::milliseconds(10),
			std::chrono::milliseconds(10),
			""
	);

	fixedDelaySchedulerRunnable.setSynchronizationObjectName("TestFixedDelaySchedulerThreadSync");

	EXPECT_CALL(*mockRunnable, run()).Times(1).WillRepeatedly(Throw(std::runtime_error("")));
	EXPECT_CALL(*mockRunnable, interrupt()).Times(1);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(std::chrono::milliseconds(10))).Times(1);

	std::thread thread([&fixedDelaySchedulerRunnable] {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		fixedDelaySchedulerRunnable.interrupt();
	});

	EXPECT_THROW(fixedDelaySchedulerRunnable.run(), std::runtime_error);
	thread.join();
}

TEST_F(FixedDelaySchedulerRunnableTest, threadStopsWorkingOnInterruptedException_TIMING) {
	FixedDelaySchedulerRunnable fixedDelaySchedulerRunnable(
			mockRunnable,
			std::chrono::milliseconds(10),
			std::chrono::milliseconds(10),
			""
	);

	fixedDelaySchedulerRunnable.setSynchronizationObjectName("TestFixedDelaySchedulerThreadSync");

	EXPECT_CALL(*mockRunnable, run()).Times(1).WillRepeatedly(Throw(InterruptedException("")));
	EXPECT_CALL(*mockRunnable, interrupt()).Times(1);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(std::chrono::milliseconds(10))).Times(1);

	std::thread thread([&fixedDelaySchedulerRunnable] {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		fixedDelaySchedulerRunnable.interrupt();
	});

	EXPECT_NO_THROW(fixedDelaySchedulerRunnable.run());
	thread.join();
}
