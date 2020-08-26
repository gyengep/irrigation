#include <chrono>
#include <gmock/gmock.h>
#include "Utils/InterruptableWait.h"

#include <iostream>
#include <thread>

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
	const std::chrono::milliseconds waitTime(500);

	for (int i = 0; i < 50; i++) {

		InterruptableWait wait;

		auto start = std::chrono::steady_clock::now();
		wait.wait_for(waitTime);
		//std::this_thread::sleep_for(waitTime);
		auto end = std::chrono::steady_clock::now();

		//EXPECT_THAT(end - start, Ge(waitTime));
		std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
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
