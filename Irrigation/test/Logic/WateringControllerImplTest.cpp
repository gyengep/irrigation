#include "WateringControllerImplTest.h"
#include "Hardware/Valves/GpioValve.h"
#include "Hardware/Valves/Impl/ZoneHandlerImpl.h"
#include <numeric>
#include <iterator>

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void WateringControllerImplTest::SetUp() {
	wateringController = make_shared<WateringControllerImpl>(
		std::make_shared<testing::NiceMock<MockZoneHandler>>()
	);
}

void WateringControllerImplTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

void WateringControllerImplTimingTest::onZoneHandlerActivate(size_t zoneId) {
	calls.push_back(make_pair(chrono::steady_clock::now(), zoneId));
}

void WateringControllerImplTimingTest::onZoneHandlerDeactivate() {
	calls.push_back(make_pair(chrono::steady_clock::now(), ZoneHandler::invalidZoneId));
}

void WateringControllerImplTimingTest::SetUp() {
	mockZoneHandler = make_shared<NiceMock<MockZoneHandler>>();
	wateringController = make_shared<WateringControllerImpl>(mockZoneHandler);

	ON_CALL(*mockZoneHandler, activate(testing::_)).WillByDefault(Invoke(this, &WateringControllerImplTimingTest::onZoneHandlerActivate));
	ON_CALL(*mockZoneHandler, deactivate()).WillByDefault(Invoke(this, &WateringControllerImplTimingTest::onZoneHandlerDeactivate));
}

void WateringControllerImplTimingTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(WateringControllerImplTest, init) {
	EXPECT_FALSE(wateringController->isWateringActive());
}

TEST_F(WateringControllerImplTest, startWithNotEmpty) {
	const std::list<std::chrono::milliseconds> runTimes {
		std::chrono::milliseconds(10),
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(0)
	};

	wateringController->startMs(runTimes);
	this_thread::yield();
	EXPECT_TRUE(wateringController->isWateringActive());
}

TEST_F(WateringControllerImplTest, stop) {
	const std::list<std::chrono::milliseconds> runTimes {
		std::chrono::milliseconds(10),
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(0)
	};

	wateringController->startMs(runTimes);
	this_thread::yield();
	EXPECT_TRUE(wateringController->isWateringActive());
	wateringController->stop();
	EXPECT_FALSE(wateringController->isWateringActive());
}

///////////////////////////////////////////////////////////////////////////////
/*
TEST_F(WateringControllerImplTimingTest, check_TIMING) {
	const std::list<std::chrono::milliseconds> runTimes {
		 std::chrono::milliseconds(100),
		 std::chrono::milliseconds(200),
		 std::chrono::milliseconds(300),
		 std::chrono::milliseconds(150),
		 std::chrono::milliseconds(250),
		 std::chrono::milliseconds(350)
	};

	const std::chrono::milliseconds sumOfTimes = accumulate(runTimes.begin(), runTimes.end(), std::chrono::milliseconds(0));

	wateringController->start(runTimes);
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

TEST_F(WateringControllerImplTimingTest, checkWithZeroAndOtherTimes_TIMING) {
	const std::list<std::chrono::milliseconds> runTimes {
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(100),
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(200),
		std::chrono::milliseconds(300),
		std::chrono::milliseconds(0)
	};

	const std::chrono::milliseconds sumOfTimes = accumulate(runTimes.begin(), runTimes.end(), std::chrono::milliseconds(0));

	wateringController->start(runTimes);
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

TEST_F(WateringControllerImplTimingTest, checkWithZeroTimes_TIMING) {
	const std::list<std::chrono::milliseconds> runTimes {
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(0),
		std::chrono::milliseconds(0)
	};

	wateringController->start(runTimes);
	while (wateringController->isWateringActive()) {
		this_thread::yield();
	}

	ASSERT_THAT(calls, SizeIs(1));
	EXPECT_THAT(calls[0].second, Eq(ZoneHandler::invalidZoneId));
}

TEST_F(WateringControllerImplTimingTest, startAndStartAgain) {
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

	wateringController->start(runTimes1);
	this_thread::sleep_for(chrono::milliseconds(300));

	wateringController->start(runTimes2);
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
*/
