#pragma once
#include <gmock/gmock.h>
#include <memory>
#include <vector>
#include "Logic/WateringController.h"

class FakeValve: public Valve {
public:
	void activate() {}
	void deactivate() {}
};

///////////////////////////////////////////////////////////////////////////////

class FakeValveFactory : public ValveFactory {
public:
	virtual std::unique_ptr<Valve> createValve(size_t) {
		return std::unique_ptr<Valve>(new FakeValve());
	}
};

///////////////////////////////////////////////////////////////////////////////

class WateringControllerTest : public ::testing::Test {
protected:

	std::unique_ptr<WateringController> wateringController;

	void checkActiveZones(time_t t, const std::vector<size_t>& requiredZones);

	virtual void SetUp();
    virtual void TearDown();
};

