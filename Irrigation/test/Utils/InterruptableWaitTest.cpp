#include <chrono>
#include <gmock/gmock.h>
#include "TestCommon/WaitFor.h"
#include "Exceptions/InterruptedException.h"
#include "Utils/InterruptableWait.h"

using namespace std;
using namespace testing;


TEST(InterruptableWaitTest, notWait) {
	auto start = chrono::steady_clock::now();
	InterruptableWait wait;
	auto end = chrono::steady_clock::now();

	EXPECT_THAT(end - start, Le(chrono::milliseconds(10)));
}

TEST(InterruptableWaitTest, interrupt) {
	auto start = chrono::steady_clock::now();
	InterruptableWait wait;
	wait.interrupt();
	auto end = chrono::steady_clock::now();

	EXPECT_THAT(end - start, Le(chrono::milliseconds(10)));
}

TEST(InterruptableWaitTest, wait) {
	const chrono::milliseconds waitTime(50);

	InterruptableWait wait;

	auto start = chrono::steady_clock::now();
	wait.wait_for(waitTime);
	auto end = chrono::steady_clock::now();

	EXPECT_THAT(end - start, Ge(waitTime));
}



void waitAndInterrupt(InterruptableWait& interruptableWait, const chrono::milliseconds& waitTime) {
	FixedWait::wait_for(waitTime);
	interruptableWait.interrupt();
}

TEST(InterruptableWaitTest, waitAndInterrupt) {
	const chrono::milliseconds interruptTime(10);
	const chrono::milliseconds waitTime(50);

	InterruptableWait wait;

	thread waitAndInterruptThread(waitAndInterrupt, ref(wait), interruptTime);

	auto start = chrono::steady_clock::now();
	EXPECT_THROW(wait.wait_for(waitTime), InterruptedException);
	auto end = chrono::steady_clock::now();

	EXPECT_THAT(end - start, Ge(interruptTime));
	EXPECT_THAT(end - start, Le(waitTime));

	waitAndInterruptThread.join();
}
