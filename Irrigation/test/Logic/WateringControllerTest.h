#pragma once
#include <gmock/gmock.h>
#include <chrono>
#include <memory>
#include <vector>
#include "Mocks/MockZoneHandler.h"
#include "Logic/WateringController.h"


///////////////////////////////////////////////////////////////////////////////

class WateringControllerTest : public ::testing::Test {
protected:

	void onZoneHandlerActivate(size_t zoneId);
	void onZoneHandlerDeactivate();

	std::shared_ptr<WateringController> wateringController;

	virtual void SetUp();
    virtual void TearDown();
};

class WateringControllerTimingTest : public ::testing::Test {
protected:

	std::vector<std::pair<std::chrono::steady_clock::time_point, size_t>> calls;

	void onZoneHandlerActivate(size_t zoneId);
	void onZoneHandlerDeactivate();

	std::shared_ptr<testing::NiceMock<MockZoneHandler>> zoneHandler;
	std::shared_ptr<WateringController> wateringController;

	virtual void SetUp();
    virtual void TearDown();
};

