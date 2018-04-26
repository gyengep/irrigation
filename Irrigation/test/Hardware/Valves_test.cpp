#include "common.h"
#include "Hardware/Valves.h"


TEST(Valves, activate) {
/*
	for (size_t i = 0; i < VALVE_COUNT; i++) {
		EXPECT_NO_THROW(Valves::getInstance().activate(i, false));
	}
*/
}

TEST(Valves, activateInvalid) {
//	EXPECT_THROW(Valves::getInstance().activate(VALVE_COUNT, true), std::out_of_range);
}
