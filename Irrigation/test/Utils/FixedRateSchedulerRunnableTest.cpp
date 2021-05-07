#include <chrono>
#include <thread>
#include "FixedRateSchedulerRunnableTest.h"
#include "Mocks/MockRunnable.h"
#include "Utils/Runnable.h"
#include "Utils/WaitObserverStore.h"

using namespace std;
using namespace testing;


void FixedRateSchedulerRunnableTest::SetUp() {
	mockWaitObserver = std::make_shared<MockWaitObserver>();
	mockRunnable = std::make_shared<MockRunnable>();

	WaitObserverStore::getInstance().insert("FixedRateSchedulerRunnableTestSync", mockWaitObserver);
}

void FixedRateSchedulerRunnableTest::TearDown() {
	WaitObserverStore::getInstance().clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(FixedRateSchedulerRunnableTest, schedulerThreadNotStarted) {
	FixedRateSchedulerRunnable fixedRateSchedulerRunnable(
			mockRunnable,
			std::chrono::milliseconds(100),
			""
	);

	fixedRateSchedulerRunnable.setSynchronizationObjectName("FixedRateSchedulerRunnableTestSync");

	EXPECT_CALL(*mockRunnable, run()).Times(0);
	EXPECT_CALL(*mockRunnable, interrupt()).Times(0);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(_)).Times(0);
}

TEST_F(FixedRateSchedulerRunnableTest, run_0_times_TIMING) {
	FixedRateSchedulerRunnable fixedRateSchedulerRunnable(
			mockRunnable,
			std::chrono::milliseconds(500),
			""
	);

	fixedRateSchedulerRunnable.setSynchronizationObjectName("FixedRateSchedulerRunnableTestSync");

	EXPECT_CALL(*mockRunnable, run()).Times(0);
	EXPECT_CALL(*mockRunnable, interrupt()).Times(1);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(_)).Times(1);

	std::thread thread([&fixedRateSchedulerRunnable] {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		fixedRateSchedulerRunnable.interrupt();
	});

	fixedRateSchedulerRunnable.run();
	thread.join();
}

TEST_F(FixedRateSchedulerRunnableTest, run_1_times_TIMING) {
	FixedRateSchedulerRunnable fixedRateSchedulerRunnable(
			mockRunnable,
			std::chrono::milliseconds(500),
			""
	);

	fixedRateSchedulerRunnable.setSynchronizationObjectName("FixedRateSchedulerRunnableTestSync");

	EXPECT_CALL(*mockRunnable, run()).Times(1);
	EXPECT_CALL(*mockRunnable, interrupt()).Times(1);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(_)).Times(2);

	std::thread thread([&fixedRateSchedulerRunnable] {
		std::this_thread::sleep_for(std::chrono::milliseconds(750));
		fixedRateSchedulerRunnable.interrupt();
	});

	fixedRateSchedulerRunnable.run();
	thread.join();
}

TEST_F(FixedRateSchedulerRunnableTest, run_more_times_TIMING) {
	FixedRateSchedulerRunnable fixedRateSchedulerRunnable(
			mockRunnable,
			std::chrono::milliseconds(100),
			""
	);

	fixedRateSchedulerRunnable.setSynchronizationObjectName("FixedRateSchedulerRunnableTestSync");

	EXPECT_CALL(*mockRunnable, run()).Times(AtLeast(3));
	EXPECT_CALL(*mockRunnable, interrupt()).Times(1);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(_)).Times(AtLeast(3));

	std::thread thread([&fixedRateSchedulerRunnable] {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		fixedRateSchedulerRunnable.interrupt();
	});

	fixedRateSchedulerRunnable.run();
	thread.join();
}

TEST_F(FixedRateSchedulerRunnableTest, fixedRate_TIMING) {
	FixedRateSchedulerRunnable fixedRateSchedulerRunnable(
			mockRunnable,
			std::chrono::milliseconds(200),
			""
	);

	fixedRateSchedulerRunnable.setSynchronizationObjectName("FixedRateSchedulerRunnableTestSync");

	auto wait100 = [] {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	};

	EXPECT_CALL(*mockRunnable, run()).
			Times(4).
			WillRepeatedly(DoAll(InvokeWithoutArgs(wait100), Return()));

	EXPECT_CALL(*mockRunnable, interrupt()).Times(1);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(_)).Times(AtLeast(5));

	std::thread thread([&fixedRateSchedulerRunnable] {
		std::this_thread::sleep_for(std::chrono::milliseconds(900));
		fixedRateSchedulerRunnable.interrupt();
	});

	fixedRateSchedulerRunnable.run();
	thread.join();
}

/*
#include "Utils/FunctionRunnable.h"
#include <iostream>

TEST_F(FixedRateSchedulerRunnableTest, aaa) {
	const std::chrono::milliseconds period(1000);

	auto start = std::chrono::steady_clock::now();
	std::chrono::milliseconds expected;

	auto func = [&start, &period, &expected] {

		expected += period;

		const auto now = std::chrono::steady_clock::now();
		const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);

		std::cout << "expected: " << expected.count() << " elapsed: " << elapsed.count() << std::endl;

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	};

	auto functionRunnable = std::make_shared<FunctionRunnable>(func);

	FixedRateSchedulerRunnable fixedRateSchedulerRunnable(
			functionRunnable,
			period,
			""
	);


//	std::thread thread([&fixedRateSchedulerRunnable] {
//		std::this_thread::sleep_for(std::chrono::milliseconds(10000));
//		fixedRateSchedulerRunnable.interrupt();
//	});

	fixedRateSchedulerRunnable.run();
//	thread.join();
}
*/
/*
TEST_F(FixedRateSchedulerRunnableTest, threadStopsWorkingOnException_TIMING) {
	FixedRateSchedulerRunnable fixedRateSchedulerRunnable(
			mockRunnable,
			std::chrono::milliseconds(10),
			std::chrono::milliseconds(10),
			""
	);

	fixedRateSchedulerRunnable.setSynchronizationObjectName("FixedRateSchedulerRunnableTestSync");

	EXPECT_CALL(*mockRunnable, run()).Times(1).WillRepeatedly(Throw(std::runtime_error("")));
	EXPECT_CALL(*mockRunnable, interrupt()).Times(1);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(std::chrono::milliseconds(10))).Times(1);

	std::thread thread([&fixedRateSchedulerRunnable] {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		fixedRateSchedulerRunnable.interrupt();
	});

	EXPECT_THROW(fixedRateSchedulerRunnable.run(), std::runtime_error);
	thread.join();
}

TEST_F(FixedRateSchedulerRunnableTest, threadStopsWorkingOnInterruptedException_TIMING) {
	FixedRateSchedulerRunnable fixedRateSchedulerRunnable(
			mockRunnable,
			std::chrono::milliseconds(10),
			std::chrono::milliseconds(10),
			""
	);

	fixedRateSchedulerRunnable.setSynchronizationObjectName("FixedRateSchedulerRunnableTestSync");

	EXPECT_CALL(*mockRunnable, run()).Times(1).WillRepeatedly(Throw(InterruptedException("")));
	EXPECT_CALL(*mockRunnable, interrupt()).Times(1);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(std::chrono::milliseconds(10))).Times(1);

	std::thread thread([&fixedRateSchedulerRunnable] {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		fixedRateSchedulerRunnable.interrupt();
	});

	EXPECT_NO_THROW(fixedRateSchedulerRunnable.run());
	thread.join();
}
*/
