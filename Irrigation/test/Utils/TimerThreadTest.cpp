#include <chrono>
#include <thread>
#include <gmock/gmock.h>
#include "Utils/TimerThread.h"

using namespace std;
using namespace testing;


class MockTimerThread : public TimerThread {
public:
	MOCK_METHOD0(onTimer, void());
};

///////////////////////////////////////////////////////////////////////////////

TEST(TimerThreadTest, timerThreadNotStarted) {
	MockTimerThread timerThread;

	EXPECT_CALL(timerThread, onTimer()).Times(0);
}

TEST(TimerThreadTest, onExecuteCalledWithoutDelay) {
	MockTimerThread timerThread;

	EXPECT_CALL(timerThread, onTimer()).Times(3);

	timerThread.schedule(chrono::milliseconds(100));
	timerThread.start();
	this_thread::sleep_for(chrono::milliseconds(350));
	timerThread.stop();
}

TEST(TimerThreadTest, onExecuteCalledWithDelay) {
	MockTimerThread timerThread;

	auto wait = [](){
		std::this_thread::sleep_for(chrono::milliseconds(50));
	};

	EXPECT_CALL(timerThread, onTimer()).Times(2).WillRepeatedly(Invoke(wait));

	timerThread.schedule(chrono::milliseconds(100));
	timerThread.start();
	this_thread::sleep_for(chrono::milliseconds(350));
	timerThread.stop();
}
