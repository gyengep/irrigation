#include <gmock/gmock.h>
#include "Exceptions/Exceptions.h"
#include "Logic/RunTime.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/ZoneConfig.h"

using namespace std;


class MockRunTime : public RunTime {
public:
	MockRunTime() {
		EXPECT_CALL(*this, destructorIsCalled()).Times(1);
	}

	MOCK_METHOD0(destructorIsCalled, bool());
	virtual ~MockRunTime() { destructorIsCalled(); }
};

TEST(RunTimeContainerTest, size) {
	RunTimeContainer runTimes;
	EXPECT_EQ(ZONE_COUNT, runTimes.size());
}


TEST(RunTimeContainerTest, id) {
	RunTimeContainer runTimes;

	unsigned id = 0;
	for (auto it = runTimes.begin(); it != runTimes.end(); ++it, ++id) {
		EXPECT_EQ(id, it->first);
	}
}

TEST(RunTimeContainerTest, at) {
	RunTimeContainer runTimes;

	unsigned i = 0;
	for (auto it = runTimes.begin(); it != runTimes.end(); ++it, ++i) {
		EXPECT_EQ(it->second, runTimes.at(i));
	}
}

TEST(RunTimeContainerTest, atConst) {
	const RunTimeContainer constRunTimes;

	unsigned i = 0;
	for (auto it = constRunTimes.begin(); it != constRunTimes.end(); ++it, ++i) {
		EXPECT_EQ(it->second, constRunTimes.at(i));
	}
}

TEST(RunTimeContainerTest, atInvalid) {
	RunTimeContainer runTimes;
	EXPECT_THROW(runTimes.at(ZONE_COUNT), NoSuchElementException);
}

TEST(RunTimeContainerTest, atConstInvalid) {
	const RunTimeContainer constRunTimes;
	EXPECT_THROW(constRunTimes.at(ZONE_COUNT), NoSuchElementException);
}


class MockRunTimeFactory : public RunTimeFactory {
public:
	virtual RunTime* createRunTime() const {
		return new MockRunTime();
	}
};

TEST(RunTimeContainerTest, destructed) {
	RunTimeContainer runTimes(new MockRunTimeFactory());
}
