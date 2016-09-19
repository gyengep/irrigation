
#include "gtest/gtest.h"

#include "Containers.h"
#include "exception.h"

TEST(RunTimeContainer, set) {
	RunTimeContainer runTimes;
	unsigned i;

	for (i = 0; i < ZONE_COUNT; i++) {
		runTimes.set(i, i * 10);
	}

	EXPECT_THROW(runTimes.set(i, 0), invalid_id);

	for (i = 0; i < ZONE_COUNT; i++) {
		EXPECT_EQ(i, runTimes.runTimes[i].first);
		EXPECT_EQ(i * 10, runTimes.runTimes[i].second);
	}
}

TEST(RunTimeContainer, get) {
	RunTimeContainer runTimes;
	unsigned i;

	for (i = 0; i < ZONE_COUNT; i++) {
		EXPECT_NO_THROW(runTimes.get(i));
	}

	EXPECT_THROW(runTimes.get(i), invalid_id);

	for (i = 0; i < ZONE_COUNT; i++) {
		EXPECT_EQ(0, runTimes.get(i));
		runTimes.set(i, i * 10);
	}

	for (i = 0; i < ZONE_COUNT; i++) {
		EXPECT_EQ(i * 10, runTimes.get(i));
	}

}
