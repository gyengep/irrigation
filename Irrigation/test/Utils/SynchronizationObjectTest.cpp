#include <chrono>
#include <list>
#include <gmock/gmock.h>
#include "TestCommon/WaitFor.h"
#include "Exceptions/IllegalStateException.h"
#include "Exceptions/InterruptedException.h"
#include "Utils/SynchronizationObject.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(SynchronizationObjectTest, synchronize) {
	const int threadCount = 100;
	const int loopCount = 10000;
	int counter = 0;

	SynchronizationObject synchronizationObject;

	auto workerFunc = [&synchronizationObject, &loopCount, &counter] {

		for (int i = 0; i < loopCount; ++i) {
			{
				auto lock = synchronizationObject.synchronize();
				counter = counter + 1;
				counter = counter - 1;
				counter = counter + 1;
			}

			std::this_thread::yield();
		}

	};

	std::vector<std::thread> workerThreads;

	for (size_t i = 0; i < threadCount; ++i) {
		workerThreads.emplace_back(workerFunc);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	for (size_t i = 0; i < workerThreads.size(); ++i) {
		workerThreads[i].join();
	}

	EXPECT_THAT(counter, Eq(threadCount * loopCount));
}

///////////////////////////////////////////////////////////////////////////////

TEST(SynchronizationObjectTest, wait_WithoutLock) {
	SynchronizationObject synchronizationObject;
	EXPECT_THROW(synchronizationObject.wait(), IllegalStateException);
}

TEST(SynchronizationObjectTest, wait_pred_WithoutLock) {
	SynchronizationObject synchronizationObject;
	EXPECT_THROW(synchronizationObject.wait([]{ return true; }), IllegalStateException);
	EXPECT_THROW(synchronizationObject.wait([]{ return false; }), IllegalStateException);
}

TEST(SynchronizationObjectTest, wait_for_WithoutLock) {
	SynchronizationObject synchronizationObject;
	EXPECT_THROW(synchronizationObject.wait_for(std::chrono::milliseconds(100)), IllegalStateException);
}

TEST(SynchronizationObjectTest, wait_for_pred_WithoutLock) {
	SynchronizationObject synchronizationObject;
	EXPECT_THROW(synchronizationObject.wait_for(std::chrono::milliseconds(100), []{ return true; }), IllegalStateException);
	EXPECT_THROW(synchronizationObject.wait_for(std::chrono::milliseconds(100), []{ return false; }), IllegalStateException);
}

///////////////////////////////////////////////////////////////////////////////

TEST(SynchronizationObjectTest, wait_for_Timeout_TIMING) {
	SynchronizationObject synchronizationObject;
	auto lock = synchronizationObject.synchronize();

	const auto start = std::chrono::steady_clock::now();
	EXPECT_NO_THROW(synchronizationObject.wait_for(std::chrono::milliseconds(200)));
	const auto end = std::chrono::steady_clock::now();

	EXPECT_THAT(end - start, Ge(std::chrono::milliseconds(100)));
}

TEST(SynchronizationObjectTest, wait_for_pred_Timeout_TIMING) {
	SynchronizationObject synchronizationObject;
	auto lock = synchronizationObject.synchronize();

	const auto start = std::chrono::steady_clock::now();
	EXPECT_NO_THROW(synchronizationObject.wait_for(std::chrono::milliseconds(200), [] { return false; }));
	const auto end = std::chrono::steady_clock::now();

	EXPECT_THAT(end - start, Ge(std::chrono::milliseconds(100)));
}

///////////////////////////////////////////////////////////////////////////////

auto waitAndNotify = [] (SynchronizationObject& synchronizationObject) {
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	synchronizationObject.notify_one();
};

auto waitAndSetInterruptAndNotify = [] (SynchronizationObject& synchronizationObject, bool& interrupted) {
	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	auto lock = synchronizationObject.synchronize();
	interrupted = true;
	synchronizationObject.notify_one();
};


TEST(SynchronizationObjectTest, wait_Interrupted_TIMING) {
	SynchronizationObject synchronizationObject;

	std::thread thread(waitAndNotify, std::ref(synchronizationObject));

	auto lock = synchronizationObject.synchronize();

	const auto start = std::chrono::steady_clock::now();
	EXPECT_THROW(synchronizationObject.wait(), InterruptedException);
	const auto end = std::chrono::steady_clock::now();

	EXPECT_THAT(end - start, Ge(std::chrono::milliseconds(100)));

	thread.join();
}


TEST(SynchronizationObjectTest, wait_for_Interrupted_TIMING) {
	SynchronizationObject synchronizationObject;

	std::thread thread(waitAndNotify, std::ref(synchronizationObject));

	auto lock = synchronizationObject.synchronize();

	const auto start = std::chrono::steady_clock::now();
	EXPECT_THROW(synchronizationObject.wait_for(std::chrono::milliseconds(1000)), InterruptedException);
	const auto end = std::chrono::steady_clock::now();

	EXPECT_THAT(end - start, Ge(std::chrono::milliseconds(100)));
	EXPECT_THAT(end - start, Le(std::chrono::milliseconds(500)));

	thread.join();
}


TEST(SynchronizationObjectTest, wait_pred_Interrupted1_TIMING) {
	SynchronizationObject synchronizationObject;

	auto lock = synchronizationObject.synchronize();

	const auto start = std::chrono::steady_clock::now();
	EXPECT_THROW(synchronizationObject.wait([]{ return true; }), InterruptedException);
	const auto end = std::chrono::steady_clock::now();

	EXPECT_THAT(end - start, Le(std::chrono::milliseconds(100)));
}

TEST(SynchronizationObjectTest, wait_pred_Interrupted2_TIMING) {
	SynchronizationObject synchronizationObject;
	bool interrupted = false;

	std::thread thread(waitAndSetInterruptAndNotify, std::ref(synchronizationObject), std::ref(interrupted));

	auto lock = synchronizationObject.synchronize();

	const auto start = std::chrono::steady_clock::now();
	EXPECT_THROW(synchronizationObject.wait([&interrupted]{ return interrupted; }), InterruptedException);
	const auto end = std::chrono::steady_clock::now();

	EXPECT_THAT(end - start, Ge(std::chrono::milliseconds(100)));

	thread.join();
}


TEST(SynchronizationObjectTest, wait_for_pred_Interrupted1_TIMING) {
	SynchronizationObject synchronizationObject;

	auto lock = synchronizationObject.synchronize();

	const auto start = std::chrono::steady_clock::now();
	EXPECT_THROW(synchronizationObject.wait_for(std::chrono::milliseconds(1000), []{ return true; }), InterruptedException);
	const auto end = std::chrono::steady_clock::now();

	EXPECT_THAT(end - start, Le(std::chrono::milliseconds(100)));
}

TEST(SynchronizationObjectTest, wait_for_pred_Interrupted2_TIMING) {
	SynchronizationObject synchronizationObject;
	bool interrupted = false;

	std::thread thread(waitAndSetInterruptAndNotify, std::ref(synchronizationObject), std::ref(interrupted));

	auto lock = synchronizationObject.synchronize();

	const auto start = std::chrono::steady_clock::now();
	EXPECT_THROW(synchronizationObject.wait_for(std::chrono::milliseconds(1000), [&interrupted]{ return interrupted; }), InterruptedException);
	const auto end = std::chrono::steady_clock::now();

	EXPECT_THAT(end - start, Ge(std::chrono::milliseconds(100)));
	EXPECT_THAT(end - start, Le(std::chrono::milliseconds(500)));

	thread.join();
}
