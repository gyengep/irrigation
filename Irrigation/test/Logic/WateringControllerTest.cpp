#include "WateringControllerTest.h"
#include "Hardware/Valves/ZoneHandler.h"
#include <numeric>

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void WateringControllerTest::SetUp() {
	wateringController = make_shared<WateringController>(ZoneHandler::getInstancePtr());
}

void WateringControllerTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

void WateringControllerTimingTest::onZoneHandlerActivate(size_t zoneId) {
	calls.push_back(make_pair(chrono::steady_clock::now(), zoneId));
}

void WateringControllerTimingTest::onZoneHandlerDeactivate() {
	calls.push_back(make_pair(chrono::steady_clock::now(), ZoneHandler::invalidZoneId));
}

void WateringControllerTimingTest::SetUp() {
	zoneHandler = make_shared<NiceMock<MockZoneHandler>>();
	wateringController = make_shared<WateringController>(zoneHandler);

	ON_CALL(*zoneHandler, activate(testing::_)).WillByDefault(Invoke(this, &WateringControllerTimingTest::onZoneHandlerActivate));
	ON_CALL(*zoneHandler, deactivate()).WillByDefault(Invoke(this, &WateringControllerTimingTest::onZoneHandlerDeactivate));
}

void WateringControllerTimingTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(WateringControllerTest, initWithNull) {
	EXPECT_THROW(WateringController(nullptr), invalid_argument);
}

TEST_F(WateringControllerTest, init) {
	EXPECT_FALSE(wateringController->isWateringActive());
}

TEST_F(WateringControllerTest, startWithNotEmpty) {
	wateringController->start(RunTimeContainer({1, 0, 0, 0, 0, 0}), 100);
	this_thread::yield();
	EXPECT_TRUE(wateringController->isWateringActive());
}

TEST_F(WateringControllerTest, stop) {
	wateringController->start(RunTimeContainer({1, 0, 0, 0, 0, 0}), 100);
	this_thread::yield();
	EXPECT_TRUE(wateringController->isWateringActive());
	wateringController->stop();
	EXPECT_FALSE(wateringController->isWateringActive());
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(WateringControllerTimingTest, timingCheck) {
	const vector<unsigned> expectedTimes { 100, 200, 300, 150, 250, 350 };
	const unsigned sumOfTimes = accumulate(expectedTimes.begin(), expectedTimes.end(), 0);
	RunTimeContainer runTimeContainer;

	for (size_t i = 0; i <runTimeContainer.size(); i++) {
		runTimeContainer.at(i)->setMilliSeconds(expectedTimes[i]);
	}

	wateringController->start(runTimeContainer, 100);
	this_thread::yield();

	const auto startTime = chrono::steady_clock::now();
	while (wateringController->isWateringActive()) {
		this_thread::yield();
	}
	const auto endTime = chrono::steady_clock::now();

	ASSERT_THAT(calls, SizeIs(7));

	EXPECT_THAT(calls[0].second, Eq(0));
	EXPECT_THAT(calls[1].second, Eq(1));
	EXPECT_THAT(calls[2].second, Eq(2));
	EXPECT_THAT(calls[3].second, Eq(3));
	EXPECT_THAT(calls[4].second, Eq(4));
	EXPECT_THAT(calls[5].second, Eq(5));
	EXPECT_THAT(calls[6].second, Eq(ZoneHandler::invalidZoneId));

	EXPECT_THAT(endTime - startTime, AllOf(Gt(chrono::milliseconds(sumOfTimes)), Le(chrono::milliseconds(sumOfTimes) * 1.2f)));
	EXPECT_THAT(calls[1].first - calls[0].first, AllOf(Gt(chrono::milliseconds(expectedTimes[0])), Le(chrono::milliseconds(expectedTimes[0]) * 1.2f)));
	EXPECT_THAT(calls[2].first - calls[1].first, AllOf(Gt(chrono::milliseconds(expectedTimes[1])), Le(chrono::milliseconds(expectedTimes[1]) * 1.2f)));
	EXPECT_THAT(calls[3].first - calls[2].first, AllOf(Gt(chrono::milliseconds(expectedTimes[2])), Le(chrono::milliseconds(expectedTimes[2]) * 1.2f)));
	EXPECT_THAT(calls[4].first - calls[3].first, AllOf(Gt(chrono::milliseconds(expectedTimes[3])), Le(chrono::milliseconds(expectedTimes[3]) * 1.2f)));
	EXPECT_THAT(calls[5].first - calls[4].first, AllOf(Gt(chrono::milliseconds(expectedTimes[4])), Le(chrono::milliseconds(expectedTimes[4]) * 1.2f)));
	EXPECT_THAT(calls[6].first - calls[5].first, AllOf(Gt(chrono::milliseconds(expectedTimes[5])), Le(chrono::milliseconds(expectedTimes[5]) * 1.2f)));
}

TEST_F(WateringControllerTimingTest, timingCheckWithZeroAndOtherTimes) {
	const vector<unsigned> expectedTimes { 0, 100, 0, 200, 300, 0 };
	const unsigned sumOfTimes = accumulate(expectedTimes.begin(), expectedTimes.end(), 0);
	RunTimeContainer runTimeContainer;

	for (size_t i = 0; i <runTimeContainer.size(); i++) {
		runTimeContainer.at(i)->setMilliSeconds(expectedTimes[i]);
	}

	wateringController->start(runTimeContainer, 100);
	this_thread::yield();

	const auto startTime = chrono::steady_clock::now();
	while (wateringController->isWateringActive()) {
		this_thread::yield();
	}
	const auto endTime = chrono::steady_clock::now();

	ASSERT_THAT(calls, SizeIs(4));

	EXPECT_THAT(calls[0].second, Eq(1));
	EXPECT_THAT(calls[1].second, Eq(3));
	EXPECT_THAT(calls[2].second, Eq(4));
	EXPECT_THAT(calls[3].second, Eq(ZoneHandler::invalidZoneId));

	EXPECT_THAT(endTime - startTime, AllOf(Gt(chrono::milliseconds(sumOfTimes)), Le(chrono::milliseconds(sumOfTimes) * 1.2f)));
	EXPECT_THAT(calls[1].first - calls[0].first, AllOf(Gt(chrono::milliseconds(expectedTimes[1])), Le(chrono::milliseconds(expectedTimes[1]) * 1.2f)));
	EXPECT_THAT(calls[2].first - calls[1].first, AllOf(Gt(chrono::milliseconds(expectedTimes[3])), Le(chrono::milliseconds(expectedTimes[3]) * 1.2f)));
	EXPECT_THAT(calls[3].first - calls[2].first, AllOf(Gt(chrono::milliseconds(expectedTimes[4])), Le(chrono::milliseconds(expectedTimes[4]) * 1.2f)));
}

TEST_F(WateringControllerTimingTest, timingCheckWithAdjustment) {
	const float adjustment = 0.7f;
	const vector<unsigned> expectedTimes { 100, 200, 300, 150, 250, 350 };
	const unsigned sumOfTimes = accumulate(expectedTimes.begin(), expectedTimes.end(), 0);
	RunTimeContainer runTimeContainer;

	for (size_t i = 0; i <runTimeContainer.size(); i++) {
		runTimeContainer.at(i)->setMilliSeconds(expectedTimes[i]);
	}

	wateringController->start(runTimeContainer, 100 * adjustment);
	this_thread::yield();

	const auto startTime = chrono::steady_clock::now();
	while (wateringController->isWateringActive()) {
		this_thread::yield();
	}
	const auto endTime = chrono::steady_clock::now();

	ASSERT_THAT(calls, SizeIs(7));

	EXPECT_THAT(calls[0].second, Eq(0));
	EXPECT_THAT(calls[1].second, Eq(1));
	EXPECT_THAT(calls[2].second, Eq(2));
	EXPECT_THAT(calls[3].second, Eq(3));
	EXPECT_THAT(calls[4].second, Eq(4));
	EXPECT_THAT(calls[5].second, Eq(5));
	EXPECT_THAT(calls[6].second, Eq(ZoneHandler::invalidZoneId));

	EXPECT_THAT(endTime - startTime, AllOf(Gt(chrono::milliseconds(sumOfTimes) * adjustment), Le(chrono::milliseconds(sumOfTimes) * adjustment * 1.2f)));
	EXPECT_THAT(calls[1].first - calls[0].first, AllOf(Gt(chrono::milliseconds(expectedTimes[0]) * adjustment), Le(chrono::milliseconds(expectedTimes[0]) * adjustment * 1.2f)));
	EXPECT_THAT(calls[2].first - calls[1].first, AllOf(Gt(chrono::milliseconds(expectedTimes[1]) * adjustment), Le(chrono::milliseconds(expectedTimes[1]) * adjustment * 1.2f)));
	EXPECT_THAT(calls[3].first - calls[2].first, AllOf(Gt(chrono::milliseconds(expectedTimes[2]) * adjustment), Le(chrono::milliseconds(expectedTimes[2]) * adjustment * 1.2f)));
	EXPECT_THAT(calls[4].first - calls[3].first, AllOf(Gt(chrono::milliseconds(expectedTimes[3]) * adjustment), Le(chrono::milliseconds(expectedTimes[3]) * adjustment * 1.2f)));
	EXPECT_THAT(calls[5].first - calls[4].first, AllOf(Gt(chrono::milliseconds(expectedTimes[4]) * adjustment), Le(chrono::milliseconds(expectedTimes[4]) * adjustment * 1.2f)));
	EXPECT_THAT(calls[6].first - calls[5].first, AllOf(Gt(chrono::milliseconds(expectedTimes[5]) * adjustment), Le(chrono::milliseconds(expectedTimes[5]) * adjustment * 1.2f)));
}

TEST_F(WateringControllerTimingTest, timingCheckWithZeroTimes) {
	RunTimeContainer runTimeContainer;

	wateringController->start(runTimeContainer, 100);
	while (wateringController->isWateringActive()) {
		this_thread::yield();
	}

	ASSERT_THAT(calls, SizeIs(1));
	EXPECT_THAT(calls[0].second, Eq(ZoneHandler::invalidZoneId));
}

TEST_F(WateringControllerTimingTest, timingCheckWithZeroAdjustment) {
	const vector<unsigned> expectedTimes { 100, 200, 300, 150, 250, 350 };
	RunTimeContainer runTimeContainer;

	for (size_t i = 0; i <runTimeContainer.size(); i++) {
		runTimeContainer.at(i)->setMilliSeconds(expectedTimes[i]);
	}

	wateringController->start(runTimeContainer, 0);
	while (wateringController->isWateringActive()) {
		this_thread::yield();
	}

	ASSERT_THAT(calls, SizeIs(1));
	EXPECT_THAT(calls[0].second, Eq(ZoneHandler::invalidZoneId));
}

TEST_F(WateringControllerTimingTest, startAndStartAgain) {
	const vector<unsigned> expectedTimes1 { 200, 200, 200, 200, 200, 200 };
	const vector<unsigned> expectedTimes2 { 100, 200, 300, 150, 250, 350 };
	RunTimeContainer runTimeContainer;

	for (size_t i = 0; i <runTimeContainer.size(); i++) {
		runTimeContainer.at(i)->setMilliSeconds(expectedTimes1[i]);
	}

	wateringController->start(runTimeContainer, 100);
	this_thread::sleep_for(chrono::milliseconds(300));

	for (size_t i = 0; i <runTimeContainer.size(); i++) {
		runTimeContainer.at(i)->setMilliSeconds(expectedTimes2[i]);
	}

	wateringController->start(runTimeContainer, 100);
	while (wateringController->isWateringActive()) {
		this_thread::yield();
	}

	ASSERT_THAT(calls, SizeIs(10));

	EXPECT_THAT(calls[0].second, Eq(0));
	EXPECT_THAT(calls[1].second, Eq(1));
	EXPECT_THAT(calls[2].second, Eq(ZoneHandler::invalidZoneId));
	EXPECT_THAT(calls[3].second, Eq(0));
	EXPECT_THAT(calls[4].second, Eq(1));
	EXPECT_THAT(calls[5].second, Eq(2));
	EXPECT_THAT(calls[6].second, Eq(3));
	EXPECT_THAT(calls[7].second, Eq(4));
	EXPECT_THAT(calls[8].second, Eq(5));
	EXPECT_THAT(calls[9].second, Eq(ZoneHandler::invalidZoneId));
}
