#include "ApplicationTest.h"
#include <chrono>
#include <thread>

using namespace std;
using namespace testing;


extern ostream& operator <<(ostream& os, const chrono::milliseconds& value);


void ApplicationTest::waitAndExit(Application* application, unsigned waitMs) {
	this_thread::sleep_for(chrono::milliseconds(waitMs));
	application->exit();
}

TEST_F(ApplicationTest, exit) {
	const unsigned waitMs = 100;
	Application application;

	thread waitAndExitThread(&ApplicationTest::waitAndExit, this, &application, waitMs);

	auto start = std::chrono::steady_clock::now();
	application.run();
	auto end = std::chrono::steady_clock::now();

	EXPECT_THAT(chrono::duration_cast<chrono::milliseconds>(end - start), Ge(chrono::milliseconds(waitMs)));

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

