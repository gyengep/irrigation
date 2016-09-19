
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

//////////////////////////////////////////////////////////////////////

TEST(StartTimeContainer, add) {
	StartTimeContainer startTimes;

	EXPECT_EQ(0, startTimes.startTimes.size());

	EXPECT_EQ(0, startTimes.add(10));
	EXPECT_EQ(1, startTimes.startTimes.size());
	EXPECT_EQ(0, startTimes.startTimes.back().first);
	EXPECT_EQ(10, startTimes.startTimes.back().second);

	EXPECT_EQ(1, startTimes.add(20));
	EXPECT_EQ(2, startTimes.startTimes.size());
	EXPECT_EQ(1, startTimes.startTimes.back().first);
	EXPECT_EQ(20, startTimes.startTimes.back().second);
}

TEST(StartTimeContainer, del) {
	StartTimeContainer startTimes;

	startTimes.add(10);
	startTimes.add(20);
	startTimes.add(30);
	startTimes.add(40);

	EXPECT_THROW(startTimes.del(5), invalid_id);

	StartTimeContainer::StartTimes::const_iterator it;

	// Delete from middle
	startTimes.del(1);

	EXPECT_EQ(3, startTimes.startTimes.size());

	it = startTimes.startTimes.begin();
	EXPECT_EQ(0, it->first);
	EXPECT_EQ(10, it->second);

	it++;
	EXPECT_EQ(2, it->first);
	EXPECT_EQ(30, it->second);

	it++;
	EXPECT_EQ(3, it->first);
	EXPECT_EQ(40, it->second);


	// Delete first
	startTimes.del(0);

	EXPECT_EQ(2, startTimes.startTimes.size());

	it = startTimes.startTimes.begin();
	EXPECT_EQ(2, it->first);
	EXPECT_EQ(30, it->second);

	it++;
	EXPECT_EQ(3, it->first);
	EXPECT_EQ(40, it->second);

	// Delete last
	startTimes.del(3);

	EXPECT_EQ(1, startTimes.startTimes.size());

	it = startTimes.startTimes.begin();
	EXPECT_EQ(2, it->first);
	EXPECT_EQ(30, it->second);
}

TEST(StartTimeContainer, get) {
	StartTimeContainer startTimes;

	startTimes.add(10);
	startTimes.add(20);
	startTimes.add(30);

	EXPECT_EQ(10, startTimes.get(0));
	EXPECT_EQ(20, startTimes.get(1));
	EXPECT_EQ(30, startTimes.get(2));
	EXPECT_THROW(startTimes.get(3), invalid_id);
}

TEST(StartTimeContainer, set) {
	StartTimeContainer startTimes;

	startTimes.add(10);
	startTimes.add(20);

	EXPECT_NO_THROW(startTimes.set(0, 100));
	EXPECT_NO_THROW(startTimes.set(1, 200));
	EXPECT_THROW(startTimes.set(2, 300), invalid_id);

	EXPECT_EQ(100, startTimes.get(0));
	EXPECT_EQ(200, startTimes.get(1));
}
