#include <gmock/gmock.h>
#include <memory>
#include "Exceptions/Exceptions.h"
#include "Hardware/Valves/ZoneHandler.h"
#include "Logic/RunTimeContainer.h"
#include "MockRunTime.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

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

TEST(RunTimeContainerTest, initWithNotCorrectInitializer) {
	EXPECT_THROW(RunTimeContainer({ 10, 11, 12, 13, 14 }), logic_error);
	EXPECT_THROW(RunTimeContainer({ 10, 11, 12, 13, 14, 15, 16 }), logic_error);
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

	for (size_t i = 0; i < runTimes.size(); ++i) {
		EXPECT_EQ(i, next(runTimes.begin(), i)->first);
	}
}

TEST(RunTimeContainerTest, at) {
	RunTimeContainer runTimes;

	for (size_t i = 0; i < runTimes.size(); ++i) {
		EXPECT_EQ(*next(runTimes.begin(), i)->second, *runTimes.at(i));
	}
}

TEST(RunTimeContainerTest, atInvalid) {
	RunTimeContainer runTimes;
	EXPECT_THROW(runTimes.at(ZoneHandler::getZoneCount()), NoSuchElementException);
}
TEST(RunTimeContainerTest, destroyed) {
	RunTimeContainer runTimes(new MockRunTimeFactory());
}
