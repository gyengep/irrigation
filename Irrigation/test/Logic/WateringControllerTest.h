#pragma once
#include <gmock/gmock.h>
#include <memory>
#include <vector>
#include "Logic/WateringController.h"



class WateringControllerTest : public ::testing::Test {
protected:

	std::unique_ptr<WateringController> wateringController;

	void checkActiveZones(time_t t, const std::vector<size_t>& requiredZones);

	virtual void SetUp();
    virtual void TearDown();
};

