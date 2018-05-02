#include "common.h"
#include "Zones_test.h"
#include "Logic/Zones.h"

ZonesTest::ZonesTest() :
	zones(MASTER_VALVE_ID, ZONE_VALVE_IDS)
{
}

ZonesTest::~ZonesTest() {
}

void ZonesTest::SetUp() {
}

void ZonesTest::TearDown() {
}


TEST_F(ZonesTest, activate) {

	for (size_t i = 0; i < ZONE_COUNT; i++) {
		EXPECT_NO_THROW(zones.activate(i, false));
	}
}

TEST_F(ZonesTest, activateInvalid) {
	EXPECT_THROW(zones.activate(ZONE_COUNT, true), std::out_of_range);
}
