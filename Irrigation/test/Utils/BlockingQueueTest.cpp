#include <thread>
#include <gmock/gmock.h>
#include "Utils/BlockingQueue.h"
#include "Exceptions/Exceptions.h"

using namespace std;
using namespace testing;


TEST(BlockinQueueTest, frontEmpty) {
	BlockingQueue<int> queue;
	EXPECT_THROW(queue.front(), std::logic_error);
}

TEST(BlockinQueueTest, popEmpty) {
	BlockingQueue<int> queue;
	EXPECT_THROW(queue.pop(), std::logic_error);
}

TEST(BlockinQueueTest, frontNotEmpty) {
	BlockingQueue<int> queue;

	queue.push(5);
	EXPECT_THAT(queue.front(), Eq(5));
	EXPECT_THAT(queue.front(), Eq(5));
	EXPECT_NO_THROW(queue.pop());
	EXPECT_THROW(queue.front(), std::logic_error);
}

TEST(BlockinQueueTest, pushAndPop) {
	BlockingQueue<int> queue;

	queue.push(5);
	EXPECT_THAT(queue.front(), Eq(5));
	EXPECT_NO_THROW(queue.pop());
	queue.push(10);
	EXPECT_THAT(queue.front(), Eq(10));
	EXPECT_NO_THROW(queue.pop());
	queue.push(20);
	queue.push(30);
	queue.push(40);
	EXPECT_THAT(queue.front(), Eq(20));
	EXPECT_NO_THROW(queue.pop());
	EXPECT_THAT(queue.front(), Eq(30));
	EXPECT_NO_THROW(queue.pop());
	EXPECT_THAT(queue.front(), Eq(40));
	EXPECT_NO_THROW(queue.pop());
	EXPECT_THROW(queue.front(), std::logic_error);
}

TEST(BlockinQueueTest, pushAndWaitForElement) {
	BlockingQueue<int> queue;
	queue.push(5);
	EXPECT_TRUE(queue.waitForElement());
}

TEST(BlockinQueueTest, interruptAndWaitForElement) {
	BlockingQueue<int> queue;
	queue.interrupt();
	EXPECT_FALSE(queue.waitForElement());
}

TEST(BlockinQueueTest, interruptAndPush) {
	BlockingQueue<int> queue;
	queue.interrupt();
	EXPECT_THROW(queue.push(80), std::logic_error);
}

TEST(BlockinQueueTest, pushInterruptWaitForElement) {
	BlockingQueue<int> queue;

	queue.push(5);
	queue.interrupt();
	EXPECT_TRUE(queue.waitForElement());
	EXPECT_THAT(queue.front(), Eq(5));
}

TEST(BlockinQueueTest, waitForElementAdd) {
	enum class Status {
		Init,
		Added
	};

	Status status = Status::Init;
	BlockingQueue<int> queue;

	auto waitAndAdd = [&queue, &status]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_THAT(status, Eq(Status::Init));
		status = Status::Added;
		queue.push(25);
	};

	thread waitAndAddThread(waitAndAdd);

	EXPECT_TRUE(queue.waitForElement());
	EXPECT_THAT(status, Eq(Status::Added));

	waitAndAddThread.join();
}

TEST(BlockinQueueTest, waitForElementInterrupt) {
	enum class Status {
		Init,
		Interrupted
	};

	Status status = Status::Init;
	BlockingQueue<int> queue;

	auto waitAndInterrupt = [&queue, &status]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_THAT(status, Eq(Status::Init));
		status = Status::Interrupted;
		queue.interrupt();
	};

	thread waitAndInterruptThread(waitAndInterrupt);

	EXPECT_FALSE(queue.waitForElement());
	EXPECT_THAT(status, Eq(Status::Interrupted));

	waitAndInterruptThread.join();
}
