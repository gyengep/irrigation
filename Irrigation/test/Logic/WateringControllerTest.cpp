#include <gmock/gmock.h>
#include <list>
#include "Logic/WateringController.h"

using namespace std;


static void checkActiveZones(WateringController& wateringController, time_t t, const vector<size_t>& requiredZones) {
	for (size_t i = 0; i < requiredZones.size(); ++i) {
		wateringController.on1SecTimer(t++);

		EXPECT_TRUE(wateringController.isWateringActive());
		EXPECT_EQ(requiredZones[i], wateringController.getActiveZoneId());
	}

	wateringController.on1SecTimer(t++);
	EXPECT_FALSE(wateringController.isWateringActive());
}

TEST(WateringController, init) {
	WateringController wateringController;
	EXPECT_FALSE(wateringController.isWateringActive());
}

TEST(WateringController, startWithNotEmpty) {
	WateringController wateringController;
	RunTimeContainer runTimeContainer;

	runTimeContainer.at(3)->setValue(1);
	wateringController.start(10, runTimeContainer);

	EXPECT_TRUE(wateringController.isWateringActive());
}

TEST(WateringController, stop) {
	WateringController wateringController;
	RunTimeContainer runTimeContainer;

	runTimeContainer.at(0)->setValue(1);
	wateringController.start(10, runTimeContainer);
	wateringController.stop();

	EXPECT_FALSE(wateringController.isWateringActive());
}

TEST(WateringController, startWithEmpty) {
	WateringController wateringController;
	RunTimeContainer runTimeContainer;

	time_t t = 10;
	wateringController.start(t, runTimeContainer);

	const vector<size_t> requiredZones { };
	checkActiveZones(wateringController, t, requiredZones);
}

TEST(WateringController, simpleSimingCheck) {
	WateringController wateringController;
	RunTimeContainer runTimeContainer;

	runTimeContainer.at(0)->setValue(2);

	time_t t = 10;
	wateringController.start(t, runTimeContainer);

	const vector<size_t> requiredZones { 0, 0 };
	checkActiveZones(wateringController, t, requiredZones);
}

TEST(WateringController, timingCheckWith0start) {
	WateringController wateringController;
	RunTimeContainer runTimeContainer;

	runTimeContainer.at(3)->setValue(2);
	runTimeContainer.at(4)->setValue(5);
	runTimeContainer.at(5)->setValue(3);

	time_t t = 100;
	wateringController.start(t, runTimeContainer);

	const vector<size_t> requiredZones { 3, 3, 4, 4, 4, 4, 4, 5, 5, 5 };
	checkActiveZones(wateringController, t, requiredZones);
}

TEST(WateringController, timingCheckWith0end) {
	WateringController wateringController;
	RunTimeContainer runTimeContainer;

	runTimeContainer.at(0)->setValue(3);
	runTimeContainer.at(2)->setValue(2);
	runTimeContainer.at(3)->setValue(1);

	time_t t = 100;
	wateringController.start(t, runTimeContainer);

	const vector<size_t> requiredZones { 0, 0, 0, 2, 2, 3 };
	checkActiveZones(wateringController, t, requiredZones);
}

