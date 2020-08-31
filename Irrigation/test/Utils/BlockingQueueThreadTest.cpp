#include <thread>
#include <gmock/gmock.h>
#include "TestCommon/cout.h"
#include "TestCommon/WaitFor.h"
#include "Mocks/MockBlockingQueueThread.h"
#include "Utils/BlockingQueueThread.h"
#include "Exceptions/Exceptions.h"

using namespace std;
using namespace testing;


TEST(BlockinQueueThreadTest, push) {
	MockBlockingQueueThread mockThread(vector<chrono::milliseconds> { chrono::milliseconds(30) });

	EXPECT_CALL(mockThread, onItemAvailable(100)).Times(1);

	mockThread.start();
	mockThread.push(100);

	WAIT_FOR(chrono::milliseconds(100));

	mockThread.stop();
}

TEST(BlockinQueueThreadTest, pushMore) {
	MockBlockingQueueThread mockThread(vector<chrono::milliseconds> { chrono::milliseconds(30) });
	Sequence seq;

	vector<chrono::steady_clock::time_point> times;
	auto saveTime = [&times](const int&) {
		times.push_back(chrono::steady_clock::now());
	};

	EXPECT_CALL(mockThread, onItemAvailable(100)).Times(1).InSequence(seq).WillRepeatedly(Invoke(saveTime));
	EXPECT_CALL(mockThread, onItemAvailable(500)).Times(1).InSequence(seq).WillRepeatedly(Invoke(saveTime));
	EXPECT_CALL(mockThread, onItemAvailable(200)).Times(1).InSequence(seq).WillRepeatedly(Invoke(saveTime));

	mockThread.start();
	mockThread.push(100);
	mockThread.push(500);
	mockThread.push(200);

	WAIT_FOR(chrono::milliseconds(100));

	mockThread.stop();

	EXPECT_THAT(times, SizeIs(3));
	EXPECT_THAT(times[1] - times[0], Le(chrono::milliseconds(10)));
	EXPECT_THAT(times[2] - times[1], Le(chrono::milliseconds(10)));
}

TEST(BlockinQueueThreadTest, wait) {
	MockBlockingQueueThread mockThread(vector<chrono::milliseconds> { chrono::milliseconds(30), chrono::milliseconds(50) });
	Sequence seq;

	vector<chrono::steady_clock::time_point> times;
	auto saveTime = [&times](const int&) {
		times.push_back(chrono::steady_clock::now());
	};

	EXPECT_CALL(mockThread, onItemAvailable(100)).Times(1).InSequence(seq).WillOnce(Invoke(saveTime));
	EXPECT_CALL(mockThread, onItemAvailable(500)).Times(1).InSequence(seq).WillOnce(DoAll(Invoke(saveTime), Throw(std::runtime_error(""))));
	EXPECT_CALL(mockThread, onError(1, _)).Times(1);
	EXPECT_CALL(mockThread, onItemAvailable(500)).Times(1).InSequence(seq).WillOnce(DoAll(Invoke(saveTime), Throw(std::runtime_error(""))));
	EXPECT_CALL(mockThread, onError(2, _)).Times(1);
	EXPECT_CALL(mockThread, onItemAvailable(500)).Times(1).InSequence(seq).WillOnce(Invoke(saveTime));
	EXPECT_CALL(mockThread, onResumed()).Times(1);
	EXPECT_CALL(mockThread, onItemAvailable(200)).Times(1).InSequence(seq).WillOnce(Invoke(saveTime));

	mockThread.start();
	mockThread.push(100);
	mockThread.push(500);
	mockThread.push(200);

	WAIT_FOR(chrono::milliseconds(100));

	mockThread.stop();

	EXPECT_THAT(times, SizeIs(5));
	EXPECT_THAT(times[1] - times[0], Le(chrono::milliseconds(10)));
	EXPECT_THAT(times[2] - times[1], AllOf(Ge(chrono::milliseconds(30)), Le(chrono::milliseconds(40))));
	EXPECT_THAT(times[3] - times[2], AllOf(Ge(chrono::milliseconds(50)), Le(chrono::milliseconds(60))));
	EXPECT_THAT(times[4] - times[3], Le(chrono::milliseconds(10)));
}

TEST(BlockinQueueThreadTest, interrupt) {
	MockBlockingQueueThread mockThread(vector<chrono::milliseconds> { chrono::milliseconds(50), chrono::milliseconds(200) });
	Sequence seq;

	vector<chrono::steady_clock::time_point> times;
	auto saveTime = [&times](const int&) {
		times.push_back(chrono::steady_clock::now());
	};

	EXPECT_CALL(mockThread, onItemAvailable(100)).Times(1).InSequence(seq).WillOnce(Invoke(saveTime));
	EXPECT_CALL(mockThread, onItemAvailable(500)).Times(1).InSequence(seq).WillOnce(DoAll(Invoke(saveTime), Throw(std::runtime_error(""))));
	EXPECT_CALL(mockThread, onError(1, _)).Times(1);
	EXPECT_CALL(mockThread, onItemAvailable(500)).Times(1).InSequence(seq).WillOnce(DoAll(Invoke(saveTime), Throw(std::runtime_error(""))));
	EXPECT_CALL(mockThread, onError(2, _)).Times(1);

	mockThread.start();
	mockThread.push(100);
	mockThread.push(500);
	mockThread.push(200);

	WAIT_FOR(chrono::milliseconds(100));

	mockThread.stop();

	EXPECT_THAT(times, SizeIs(3));
	EXPECT_THAT(times[1] - times[0], Le(chrono::milliseconds(10)));
	EXPECT_THAT(times[2] - times[1], AllOf(Ge(chrono::milliseconds(50)), Le(chrono::milliseconds(60))));
}
