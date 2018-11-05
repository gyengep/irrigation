#include <gmock/gmock.h>
#include "Exceptions/Exceptions.h"
#include "Hardware/Valves/ZoneHandler.h"
#include "Logic/RunTime.h"
#include "Logic/RunTimeContainer.h"

using namespace std;
using namespace testing;


class MockRunTime : public RunTime {
public:
	MockRunTime() {
		EXPECT_CALL(*this, destructorIsCalled()).Times(1);
	}

	MOCK_METHOD0(destructorIsCalled, bool());
	virtual ~MockRunTime() { destructorIsCalled(); }
};

TEST(RunTimeContainerTest, initDefault) {
	const RunTimeContainer runTimes;

	EXPECT_THAT(*runTimes.at(0), Eq(RunTime()));
	EXPECT_THAT(*runTimes.at(1), Eq(RunTime()));
	EXPECT_THAT(*runTimes.at(2), Eq(RunTime()));
	EXPECT_THAT(*runTimes.at(3), Eq(RunTime()));
	EXPECT_THAT(*runTimes.at(4), Eq(RunTime()));
	EXPECT_THAT(*runTimes.at(5), Eq(RunTime()));
}

TEST(RunTimeContainerTest, initWithInitializer) {
	const RunTimeContainer runTimes{ 10, 11, 12, 13, 14, 15 };

	EXPECT_THAT(*runTimes.at(0), Eq(RunTime(10)));
	EXPECT_THAT(*runTimes.at(1), Eq(RunTime(11)));
	EXPECT_THAT(*runTimes.at(2), Eq(RunTime(12)));
	EXPECT_THAT(*runTimes.at(3), Eq(RunTime(13)));
	EXPECT_THAT(*runTimes.at(4), Eq(RunTime(14)));
	EXPECT_THAT(*runTimes.at(5), Eq(RunTime(15)));
}

TEST(RunTimeContainerTest, initCopy) {
	const RunTimeContainer source{ 10, 11, 12, 13, 14, 15 };
	const RunTimeContainer runTimes(source);

	EXPECT_THAT(*runTimes.at(0), Eq(RunTime(10)));
	EXPECT_THAT(*runTimes.at(1), Eq(RunTime(11)));
	EXPECT_THAT(*runTimes.at(2), Eq(RunTime(12)));
	EXPECT_THAT(*runTimes.at(3), Eq(RunTime(13)));
	EXPECT_THAT(*runTimes.at(4), Eq(RunTime(14)));
	EXPECT_THAT(*runTimes.at(5), Eq(RunTime(15)));
}

TEST(RunTimeContainerTest, copy) {
	const RunTimeContainer source{ 10, 11, 12, 13, 14, 15 };
	RunTimeContainer runTimes;
	runTimes = source;

	EXPECT_THAT(*runTimes.at(0), Eq(RunTime(10)));
	EXPECT_THAT(*runTimes.at(1), Eq(RunTime(11)));
	EXPECT_THAT(*runTimes.at(2), Eq(RunTime(12)));
	EXPECT_THAT(*runTimes.at(3), Eq(RunTime(13)));
	EXPECT_THAT(*runTimes.at(4), Eq(RunTime(14)));
	EXPECT_THAT(*runTimes.at(5), Eq(RunTime(15)));
}

TEST(RunTimeContainerTest, equals) {
	EXPECT_TRUE(RunTimeContainer({ 10, 11, 12, 13, 14, 15 }) == RunTimeContainer({ 10, 11, 12, 13, 14, 15 }));
	EXPECT_FALSE(RunTimeContainer({ 10, 11, 12, 13, 14, 15 }) == RunTimeContainer({ 20, 21, 22, 23, 24, 25 }));
	EXPECT_FALSE(RunTimeContainer({ 10, 11, 12, 13, 14, 15 }) == RunTimeContainer({ 20, 11, 12, 13, 14, 15 }));
	EXPECT_FALSE(RunTimeContainer({ 10, 11, 12, 13, 14, 15 }) == RunTimeContainer({ 20, 11, 12, 13, 14, 15 }));
	EXPECT_FALSE(RunTimeContainer({ 10, 11, 12, 13, 14, 15 }) == RunTimeContainer({ 10, 21, 12, 13, 14, 15 }));
	EXPECT_FALSE(RunTimeContainer({ 10, 11, 12, 13, 14, 15 }) == RunTimeContainer({ 10, 11, 22, 13, 14, 15 }));
	EXPECT_FALSE(RunTimeContainer({ 10, 11, 12, 13, 14, 15 }) == RunTimeContainer({ 10, 11, 12, 23, 14, 15 }));
	EXPECT_FALSE(RunTimeContainer({ 10, 11, 12, 13, 14, 15 }) == RunTimeContainer({ 10, 11, 12, 13, 24, 15 }));
	EXPECT_FALSE(RunTimeContainer({ 10, 11, 12, 13, 14, 15 }) == RunTimeContainer({ 10, 11, 12, 13, 14, 25 }));
}

TEST(RunTimeContainerTest, size) {
	RunTimeContainer runTimes;
	EXPECT_EQ(ZoneHandler::getZoneCount(), runTimes.size());
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
		EXPECT_EQ(it->second.get(), runTimes.at(i));
	}
}

TEST(RunTimeContainerTest, atConst) {
	const RunTimeContainer constRunTimes;

	unsigned i = 0;
	for (auto it = constRunTimes.begin(); it != constRunTimes.end(); ++it, ++i) {
		EXPECT_EQ(it->second.get(), constRunTimes.at(i));
	}
}

TEST(RunTimeContainerTest, atInvalid) {
	RunTimeContainer runTimes;
	EXPECT_THROW(runTimes.at(ZoneHandler::getZoneCount()), NoSuchElementException);
}

TEST(RunTimeContainerTest, atConstInvalid) {
	const RunTimeContainer constRunTimes;
	EXPECT_THROW(constRunTimes.at(ZoneHandler::getZoneCount()), NoSuchElementException);
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
