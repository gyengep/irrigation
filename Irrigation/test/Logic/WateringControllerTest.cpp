#include "WateringControllerTest.h"
#include "Logic/RunTimeImpl.h"
#include "Logic/RunTimeContainerImpl.h"
#include "Hardware/Valves/GpioValve.h"
#include "Hardware/Valves/ZoneHandlerImpl.h"
#include <numeric>
#include <iterator>

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void WateringControllerTest::SetUp() {
	wateringController = make_shared<WateringController>(
		std::make_shared<testing::NiceMock<MockZoneHandler>>()
	);
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
	const std::list<std::chrono::milliseconds> runTimes {
		std::chrono::milliseconds(1),
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(0)
	};

	wateringController->start(runTimes, 100);
	this_thread::yield();
	EXPECT_TRUE(wateringController->isWateringActive());
}

TEST_F(WateringControllerTest, stop) {
	const std::list<std::chrono::milliseconds> runTimes {
		std::chrono::milliseconds(1),
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(0)
	};

	wateringController->start(runTimes, 100);
	this_thread::yield();
	EXPECT_TRUE(wateringController->isWateringActive());
	wateringController->stop();
	EXPECT_FALSE(wateringController->isWateringActive());
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(WateringControllerTimingTest, check_TIMING) {
	const std::list<std::chrono::milliseconds> runTimes {
		 std::chrono::milliseconds(100),
		 std::chrono::milliseconds(200),
		 std::chrono::milliseconds(300),
		 std::chrono::milliseconds(150),
		 std::chrono::milliseconds(250),
		 std::chrono::milliseconds(350)
	};

	const std::chrono::milliseconds sumOfTimes = accumulate(runTimes.begin(), runTimes.end(), std::chrono::milliseconds(0));

	wateringController->start(runTimes, 100);
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

	EXPECT_THAT(endTime - startTime, AllOf(Gt(sumOfTimes), Le(sumOfTimes * 1.2f)));

	EXPECT_THAT(calls[1].first - calls[0].first, AllOf(Gt(*next(runTimes.begin(), 0)), Le(*next(runTimes.begin(), 0) * 1.2f)));
	EXPECT_THAT(calls[2].first - calls[1].first, AllOf(Gt(*next(runTimes.begin(), 1)), Le(*next(runTimes.begin(), 1) * 1.2f)));
	EXPECT_THAT(calls[3].first - calls[2].first, AllOf(Gt(*next(runTimes.begin(), 2)), Le(*next(runTimes.begin(), 2) * 1.2f)));
	EXPECT_THAT(calls[4].first - calls[3].first, AllOf(Gt(*next(runTimes.begin(), 3)), Le(*next(runTimes.begin(), 3) * 1.2f)));
	EXPECT_THAT(calls[5].first - calls[4].first, AllOf(Gt(*next(runTimes.begin(), 4)), Le(*next(runTimes.begin(), 4) * 1.2f)));
	EXPECT_THAT(calls[6].first - calls[5].first, AllOf(Gt(*next(runTimes.begin(), 5)), Le(*next(runTimes.begin(), 5) * 1.2f)));
}

TEST_F(WateringControllerTimingTest, checkWithZeroAndOtherTimes_TIMING) {
	const std::list<std::chrono::milliseconds> runTimes {
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(100),
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(200),
		std::chrono::milliseconds(300),
		std::chrono::milliseconds(0)
	};

	const std::chrono::milliseconds sumOfTimes = accumulate(runTimes.begin(), runTimes.end(), std::chrono::milliseconds(0));

	wateringController->start(runTimes, 100);
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
	EXPECT_THAT(calls[1].first - calls[0].first, AllOf(Gt(*next(runTimes.begin(), 1)), Le(*next(runTimes.begin(), 1) * 1.2f)));
	EXPECT_THAT(calls[2].first - calls[1].first, AllOf(Gt(*next(runTimes.begin(), 3)), Le(*next(runTimes.begin(), 3) * 1.2f)));
	EXPECT_THAT(calls[3].first - calls[2].first, AllOf(Gt(*next(runTimes.begin(), 4)), Le(*next(runTimes.begin(), 4) * 1.2f)));
}

TEST_F(WateringControllerTimingTest, checkWithAdjustment_TIMING) {
	const float adjustment = 0.7f;

	const std::list<std::chrono::milliseconds> runTimes {
		std::chrono::milliseconds(100),
		std::chrono::milliseconds(200),
		std::chrono::milliseconds(300),
		std::chrono::milliseconds(150),
		std::chrono::milliseconds(250),
		std::chrono::milliseconds(350)
	};

	const std::chrono::milliseconds sumOfTimes = accumulate(runTimes.begin(), runTimes.end(), std::chrono::milliseconds(0));

	wateringController->start(runTimes, 100 * adjustment);
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
	EXPECT_THAT(calls[1].first - calls[0].first, AllOf(Gt(*next(runTimes.begin(), 0) * adjustment), Le(*next(runTimes.begin(), 0) * adjustment * 1.2f)));
	EXPECT_THAT(calls[2].first - calls[1].first, AllOf(Gt(*next(runTimes.begin(), 1) * adjustment), Le(*next(runTimes.begin(), 1) * adjustment * 1.2f)));
	EXPECT_THAT(calls[3].first - calls[2].first, AllOf(Gt(*next(runTimes.begin(), 2) * adjustment), Le(*next(runTimes.begin(), 2) * adjustment * 1.2f)));
	EXPECT_THAT(calls[4].first - calls[3].first, AllOf(Gt(*next(runTimes.begin(), 3) * adjustment), Le(*next(runTimes.begin(), 3) * adjustment * 1.2f)));
	EXPECT_THAT(calls[5].first - calls[4].first, AllOf(Gt(*next(runTimes.begin(), 4) * adjustment), Le(*next(runTimes.begin(), 4) * adjustment * 1.2f)));
	EXPECT_THAT(calls[6].first - calls[5].first, AllOf(Gt(*next(runTimes.begin(), 5) * adjustment), Le(*next(runTimes.begin(), 5) * adjustment * 1.2f)));
}

TEST_F(WateringControllerTimingTest, checkWithZeroTimes_TIMING) {
	const std::list<std::chrono::milliseconds> runTimes {
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(0)
	};

	wateringController->start(runTimes, 100);
	while (wateringController->isWateringActive()) {
		this_thread::yield();
	}

	ASSERT_THAT(calls, SizeIs(1));
	EXPECT_THAT(calls[0].second, Eq(ZoneHandler::invalidZoneId));
}

TEST_F(WateringControllerTimingTest, checkWithZeroAdjustment_TIMING) {
	const std::list<std::chrono::milliseconds> runTimes {
		std::chrono::milliseconds(100),
		std::chrono::milliseconds(200),
		std::chrono::milliseconds(300),
		std::chrono::milliseconds(150),
		std::chrono::milliseconds(250),
		std::chrono::milliseconds(350)
	};

	wateringController->start(runTimes, 0);
	while (wateringController->isWateringActive()) {
		this_thread::yield();
	}

	ASSERT_THAT(calls, SizeIs(1));
	EXPECT_THAT(calls[0].second, Eq(ZoneHandler::invalidZoneId));
}

TEST_F(WateringControllerTimingTest, startAndStartAgain) {
	const std::list<std::chrono::milliseconds> runTimes1 {
		std::chrono::milliseconds(200),
		std::chrono::milliseconds(200),
		std::chrono::milliseconds(200),
		std::chrono::milliseconds(200),
		std::chrono::milliseconds(200),
		std::chrono::milliseconds(200)
	};

	const std::list<std::chrono::milliseconds> runTimes2 {
		std::chrono::milliseconds(100),
		std::chrono::milliseconds(200),
		std::chrono::milliseconds(300),
		std::chrono::milliseconds(150),
		std::chrono::milliseconds(250),
		std::chrono::milliseconds(350)
	};

	wateringController->start(runTimes1, 100);
	this_thread::sleep_for(chrono::milliseconds(300));

	wateringController->start(runTimes2, 100);
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
