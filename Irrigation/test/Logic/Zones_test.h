#pragma once
#include <gmock/gmock.h>

#include "Logic/Zones.h"


class ZonesTest : public ::testing::Test {
protected:

	Zones zones;

    ZonesTest();
    virtual ~ZonesTest();
    virtual void SetUp();
    virtual void TearDown();
};
