#include "ApplicationTest.h"
#include <chrono>
#include <thread>
#include "Mocks/MockApplication.h"

using namespace testing;


void ApplicationTest::waitAndExit(Application* application, unsigned waitMs) {
	std::this_thread::sleep_for(std::chrono::milliseconds(waitMs + 10));
	application->exit();
}

TEST_F(ApplicationTest, exit) {
	const unsigned waitMs = 100;
	Application application;

	auto start = std::chrono::high_resolution_clock::now();
	std::thread waitAndExitThread(&ApplicationTest::waitAndExit, this, &application, waitMs);

	application.run();
	auto end = std::chrono::high_resolution_clock::now();

	EXPECT_THAT(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(), Ge(waitMs));

	waitAndExitThread.join();
}

TEST_F(ApplicationTest, initAndTerminate) {
	const unsigned waitMs = 1;
	MockApplication application;

	EXPECT_CALL(application, onInitialize()).Times(1);
	EXPECT_CALL(application, onTerminate()).Times(1);

	std::thread waitAndExitThread(&ApplicationTest::waitAndExit, this, &application, waitMs);
	application.run();

	waitAndExitThread.join();
}

