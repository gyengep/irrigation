#include <chrono>
#include <gmock/gmock.h>
#include "TestCommon/WaitFor.h"
#include "Exceptions/InterruptedException.h"
#include "Utils/IncrementalWait.h"

using namespace std;
using namespace testing;


TEST(IncrementalWaitTest, init) {
	EXPECT_THROW(IncrementalWait(vector<chrono::milliseconds>()), runtime_error);
}

TEST(IncrementalWaitTest, notWait) {
	auto start = chrono::steady_clock::now();

	IncrementalWait wait(vector<chrono::milliseconds>({
		chrono::milliseconds(10),
		chrono::milliseconds(20),
		chrono::milliseconds(30)
	}));

	auto end = chrono::steady_clock::now();

	EXPECT_THAT(end - start, Le(chrono::milliseconds(10)));
}

TEST(IncrementalWaitTest, interrupt) {
	auto start = chrono::steady_clock::now();

	IncrementalWait wait(vector<chrono::milliseconds>({
		chrono::milliseconds(10),
		chrono::milliseconds(20),
		chrono::milliseconds(30)
	}));

	wait.interrupt();
	auto end = chrono::steady_clock::now();

	EXPECT_THAT(end - start, Le(chrono::milliseconds(10)));
}

TEST(IncrementalWaitTest, wait) {

	IncrementalWait wait(vector<chrono::milliseconds>({
		chrono::milliseconds(10),
		chrono::milliseconds(20),
		chrono::milliseconds(30)
	}));

	auto time0 = chrono::steady_clock::now();
	wait.wait();
	auto time1 = chrono::steady_clock::now();
	wait.wait();
	auto time2 = chrono::steady_clock::now();
	wait.wait();
	auto time3 = chrono::steady_clock::now();
	wait.wait();
	auto time4 = chrono::steady_clock::now();

	EXPECT_THAT(time1 - time0, AllOf(Ge(chrono::milliseconds(10)), Le(chrono::milliseconds(20))));
	EXPECT_THAT(time2 - time1, AllOf(Ge(chrono::milliseconds(20)), Le(chrono::milliseconds(30))));
	EXPECT_THAT(time3 - time2, AllOf(Ge(chrono::milliseconds(30)), Le(chrono::milliseconds(40))));
	EXPECT_THAT(time4 - time3, AllOf(Ge(chrono::milliseconds(30)), Le(chrono::milliseconds(40))));
}

TEST(IncrementalWaitTest, reset) {

	IncrementalWait wait(vector<chrono::milliseconds>({
		chrono::milliseconds(10),
		chrono::milliseconds(20),
		chrono::milliseconds(30)
	}));

	auto time0 = chrono::steady_clock::now();
	wait.wait();
	auto time1 = chrono::steady_clock::now();
	wait.wait();

	wait.resetWaitTime();
	auto time2 = chrono::steady_clock::now();
	wait.wait();
	auto time3 = chrono::steady_clock::now();
	wait.wait();
	auto time4 = chrono::steady_clock::now();
	wait.wait();
	auto time5 = chrono::steady_clock::now();
	wait.wait();
	auto time6 = chrono::steady_clock::now();

	EXPECT_THAT(time1 - time0, AllOf(Ge(chrono::milliseconds(10)), Le(chrono::milliseconds(20))));
	EXPECT_THAT(time2 - time1, AllOf(Ge(chrono::milliseconds(20)), Le(chrono::milliseconds(30))));

	EXPECT_THAT(time3 - time2, AllOf(Ge(chrono::milliseconds(10)), Le(chrono::milliseconds(20))));
	EXPECT_THAT(time4 - time3, AllOf(Ge(chrono::milliseconds(20)), Le(chrono::milliseconds(30))));
	EXPECT_THAT(time5 - time4, AllOf(Ge(chrono::milliseconds(30)), Le(chrono::milliseconds(40))));
	EXPECT_THAT(time6 - time5, AllOf(Ge(chrono::milliseconds(30)), Le(chrono::milliseconds(40))));
}

TEST(IncrementalWaitTest, waitAndInterrupt) {
	const chrono::milliseconds interruptTime(50);

	IncrementalWait wait(vector<chrono::milliseconds>({
		chrono::milliseconds(30),
		chrono::milliseconds(100),
		chrono::milliseconds(200)
	}));

	auto waitAndInterrupt = [] (IncrementalWait& incrementalWait, const chrono::milliseconds& waitTime) {
		WAIT_FOR(waitTime);
		incrementalWait.interrupt();
	};


	auto time0 = chrono::steady_clock::now();
	wait.wait();
	auto time1 = chrono::steady_clock::now();

	EXPECT_THAT(time1 - time0, AllOf(Ge(chrono::milliseconds(30)), Le(chrono::milliseconds(40))));

	thread waitAndInterruptThread(waitAndInterrupt, ref(wait), interruptTime);
	auto time2 = chrono::steady_clock::now();
	EXPECT_THROW(wait.wait(), InterruptedException);
	auto time3 = chrono::steady_clock::now();

	EXPECT_THAT(time3 - time2, AllOf(Ge(chrono::milliseconds(40)), Le(chrono::milliseconds(60))));

	waitAndInterruptThread.join();
}
