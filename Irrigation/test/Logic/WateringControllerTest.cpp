#include "WateringControllerTest.h"

using namespace std;


void WateringControllerTest::SetUp() {
	shared_ptr<GpioHandler> gpioHandler;
	shared_ptr<Valves> valves(new Valves(gpioHandler));
	shared_ptr<ZoneHandler> zoneHandler(new ZoneHandler(valves));
	wateringController.reset(new WateringController(zoneHandler));
}

void WateringControllerTest::TearDown() {
}


void WateringControllerTest::checkActiveZones(time_t t, const vector<size_t>& requiredZones) {
	for (size_t i = 0; i < requiredZones.size(); ++i) {
		wateringController->on1SecTimer(t++);

		EXPECT_TRUE(wateringController->isWateringActive());
		EXPECT_EQ(requiredZones[i], wateringController->getActiveZoneId());
	}

	wateringController->on1SecTimer(t++);
	EXPECT_FALSE(wateringController->isWateringActive());
}

TEST_F(WateringControllerTest, initWithNull) {
	EXPECT_THROW(WateringController(shared_ptr<ZoneHandler>()), invalid_argument);
}

TEST_F(WateringControllerTest, init) {
	EXPECT_FALSE(wateringController->isWateringActive());
}

TEST_F(WateringControllerTest, startWithNotEmpty) {
	RunTimeContainer runTimeContainer;

	runTimeContainer.at(3)->setSeconds(1);
	wateringController->start(10, runTimeContainer);

	EXPECT_TRUE(wateringController->isWateringActive());
}

TEST_F(WateringControllerTest, stop) {
	RunTimeContainer runTimeContainer;

	runTimeContainer.at(0)->setSeconds(1);
	wateringController->start(10, runTimeContainer);
	wateringController->stop();

	EXPECT_FALSE(wateringController->isWateringActive());
}

TEST_F(WateringControllerTest, startWithEmpty) {
	RunTimeContainer runTimeContainer;

	time_t t = time(nullptr);
	wateringController->start(t, runTimeContainer);

	const vector<size_t> requiredZones { };
	checkActiveZones(t, requiredZones);
}

TEST_F(WateringControllerTest, simpleSimingCheck) {
	RunTimeContainer runTimeContainer;

	runTimeContainer.at(0)->setSeconds(2);

	time_t t = time(nullptr);
	wateringController->start(t, runTimeContainer);

	const vector<size_t> requiredZones { 0, 0 };
	checkActiveZones(t, requiredZones);
}

TEST_F(WateringControllerTest, timingCheckWith0start) {
	RunTimeContainer runTimeContainer;

	runTimeContainer.at(3)->setSeconds(2);
	runTimeContainer.at(4)->setSeconds(5);
	runTimeContainer.at(5)->setSeconds(3);

	time_t t = time(nullptr);
	wateringController->start(t, runTimeContainer);

	const vector<size_t> requiredZones { 3, 3, 4, 4, 4, 4, 4, 5, 5, 5 };
	checkActiveZones(t, requiredZones);
}

TEST_F(WateringControllerTest, timingCheckWith0end) {
	RunTimeContainer runTimeContainer;

	runTimeContainer.at(0)->setSeconds(3);
	runTimeContainer.at(2)->setSeconds(2);
	runTimeContainer.at(3)->setSeconds(1);

	time_t t = time(nullptr);
	wateringController->start(t, runTimeContainer);

	const vector<size_t> requiredZones { 0, 0, 0, 2, 2, 3 };
	checkActiveZones(t, requiredZones);
}

TEST_F(WateringControllerTest, timingAdjust) {
	RunTimeContainer runTimeContainer;

	runTimeContainer.at(0)->setSeconds(10);
	runTimeContainer.at(2)->setSeconds(20);

	time_t t = time(nullptr);
	wateringController->start(t, runTimeContainer, 30);

	const vector<size_t> requiredZones { 0, 0, 0, 2, 2, 2, 2, 2, 2 };
	checkActiveZones(t, requiredZones);
}
