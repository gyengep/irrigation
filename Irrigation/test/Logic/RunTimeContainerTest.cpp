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

	EXPECT_THAT(runTimes.at(0), Pointee(RunTime()));
	EXPECT_THAT(runTimes.at(1), Pointee(RunTime()));
	EXPECT_THAT(runTimes.at(2), Pointee(RunTime()));
	EXPECT_THAT(runTimes.at(3), Pointee(RunTime()));
	EXPECT_THAT(runTimes.at(4), Pointee(RunTime()));
	EXPECT_THAT(runTimes.at(5), Pointee(RunTime()));
}

TEST(RunTimeContainerTest, initWithInitializer) {
	const RunTimeContainer runTimes{ 10, 11, 12, 13, 14, 15 };

	EXPECT_THAT(runTimes.at(0), Pointee(RunTime(10)));
	EXPECT_THAT(runTimes.at(1), Pointee(RunTime(11)));
	EXPECT_THAT(runTimes.at(2), Pointee(RunTime(12)));
	EXPECT_THAT(runTimes.at(3), Pointee(RunTime(13)));
	EXPECT_THAT(runTimes.at(4), Pointee(RunTime(14)));
	EXPECT_THAT(runTimes.at(5), Pointee(RunTime(15)));
}

TEST(RunTimeContainerTest, initWithNotCorrectInitializer) {
	EXPECT_THROW(RunTimeContainer({ 10, 11, 12, 13, 14 }), logic_error);
	EXPECT_THROW(RunTimeContainer({ 10, 11, 12, 13, 14, 15, 16 }), logic_error);
}

TEST(RunTimeContainerTest, initCopy) {
	const RunTimeContainer source{ 10, 11, 12, 13, 14, 15 };
	const RunTimeContainer runTimes(source);

	EXPECT_THAT(runTimes.at(0), Pointee(RunTime(10)));
	EXPECT_THAT(runTimes.at(1), Pointee(RunTime(11)));
	EXPECT_THAT(runTimes.at(2), Pointee(RunTime(12)));
	EXPECT_THAT(runTimes.at(3), Pointee(RunTime(13)));
	EXPECT_THAT(runTimes.at(4), Pointee(RunTime(14)));
	EXPECT_THAT(runTimes.at(5), Pointee(RunTime(15)));
}

TEST(RunTimeContainerTest, copy) {
	const RunTimeContainer source{ 10, 11, 12, 13, 14, 15 };
	RunTimeContainer runTimes;
	runTimes = source;

	EXPECT_THAT(runTimes.at(0), Pointee(RunTime(10)));
	EXPECT_THAT(runTimes.at(1), Pointee(RunTime(11)));
	EXPECT_THAT(runTimes.at(2), Pointee(RunTime(12)));
	EXPECT_THAT(runTimes.at(3), Pointee(RunTime(13)));
	EXPECT_THAT(runTimes.at(4), Pointee(RunTime(14)));
	EXPECT_THAT(runTimes.at(5), Pointee(RunTime(15)));
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
	EXPECT_THAT(runTimes, SizeIs(ZoneHandler::getZoneCount()));
}

TEST(RunTimeContainerTest, id) {
	RunTimeContainer runTimes;

	for (size_t i = 0; i < runTimes.size(); ++i) {
		EXPECT_THAT(next(runTimes.begin(), i)->first, Eq(i));
	}
}

TEST(RunTimeContainerTest, at) {
	RunTimeContainer runTimes;

	for (size_t i = 0; i < runTimes.size(); ++i) {
		EXPECT_THAT(runTimes.at(i), Eq(next(runTimes.begin(), i)->second));
	}
}

TEST(RunTimeContainerTest, atInvalid) {
	RunTimeContainer runTimes;
	EXPECT_THROW(runTimes.at(ZoneHandler::getZoneCount()), NoSuchElementException);
}

TEST(RunTimeContainerTest, destroyed) {
	RunTimeContainer runTimes(unique_ptr<RunTimeFactory>(new MockRunTimeFactory()));
}
