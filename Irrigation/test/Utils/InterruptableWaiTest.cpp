#include <chrono>
#include <gmock/gmock.h>
#include "Utils/InterruptableWait.h"

using namespace std;
using namespace testing;


TEST(InterruptableWaitTest, notWait) {
	auto start = std::chrono::steady_clock::now();
	InterruptableWait wait;
	auto end = std::chrono::steady_clock::now();

	EXPECT_THAT(end - start, Le(chrono::milliseconds(10)));
}

TEST(InterruptableWaitTest, finish) {
	auto start = std::chrono::steady_clock::now();
	InterruptableWait wait;
	wait.finish();
	auto end = std::chrono::steady_clock::now();

	EXPECT_THAT(end - start, Le(chrono::milliseconds(10)));
}

TEST(InterruptableWaitTest, wait) {

	for (int i = 0; i < 20; i++) {
		InterruptableWait wait;

		auto start = std::chrono::steady_clock::now();
		wait.wait_for(chrono::milliseconds(50));
		auto end = std::chrono::steady_clock::now();

		EXPECT_THAT(end - start, Ge(chrono::milliseconds(50)));
	}
}
/*
TEST(InterruptableWaitTest, waitAndTerminate) {
	auto start = std::chrono::steady_clock::now();
	InterruptableWait wait;
	wait.wait_for(chrono::milliseconds(50));
	auto end = std::chrono::steady_clock::now();

	EXPECT_THAT(end - start, Gt(chrono::milliseconds(50)));
}
*/
