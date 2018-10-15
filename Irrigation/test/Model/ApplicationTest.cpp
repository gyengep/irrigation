#include "ApplicationTest.h"
#include <chrono>
#include <thread>

using namespace std;
using namespace testing;



void ApplicationTest::waitAndExit(Application* application, unsigned waitMs) {
	this_thread::sleep_for(chrono::milliseconds(waitMs + 10));
	application->exit();
}

TEST_F(ApplicationTest, exit) {
	const unsigned waitMs = 100;
	Application application;

	auto start = chrono::high_resolution_clock::now();
	thread waitAndExitThread(&ApplicationTest::waitAndExit, this, &application, waitMs);

	application.run();
	auto end = chrono::high_resolution_clock::now();

	EXPECT_THAT(chrono::duration_cast<chrono::milliseconds>(end - start).count(), Ge(waitMs));

	waitAndExitThread.join();
}

TEST_F(ApplicationTest, initAndTerminate) {
	const unsigned waitMs = 1;
	MockApplication application;

	EXPECT_CALL(application, onInitialize()).Times(1);
	EXPECT_CALL(application, onTerminate()).Times(1);

	thread waitAndExitThread(&ApplicationTest::waitAndExit, this, &application, waitMs);
	application.run();

	waitAndExitThread.join();
}

