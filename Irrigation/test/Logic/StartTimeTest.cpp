#include <gmock/gmock.h>
#include <stdexcept>
#include "Logic/StartTime.h"

using namespace std;


TEST(StartTimeTest, init) {
	StartTime startTime;
	EXPECT_EQ(0, startTime.getValue());
}

TEST(StartTimeTest, initWithHourMinSec) {
	StartTime startTime(1, 30, 10);

	EXPECT_EQ(1*3600 + 30*60 + 10, startTime.getValue());
}

TEST(StartTimeTest, setValue) {
	StartTime startTime;
	startTime.setValue(50);

	EXPECT_EQ(50, startTime.getValue());
}

TEST(StartTimeTest, equal) {
	StartTime startTime;

	EXPECT_TRUE(StartTime() == startTime);

	startTime.setValue(3605);
	EXPECT_FALSE(StartTime() == startTime);
	EXPECT_FALSE(StartTime(1, 0, 4) == startTime);
	EXPECT_TRUE(StartTime(1, 0, 5) == startTime);
	EXPECT_FALSE(StartTime(1, 0, 6) == startTime);
}

TEST(StartTimeTest, toString) {
	StartTime startTime;
	startTime.setValue(2465);
	EXPECT_EQ("2465", startTime.toString());
}

TEST(StartTimeTest, fromString) {
	StartTime startTime;
	startTime.fromString("2465");
	EXPECT_EQ(2465, startTime.getValue());
}

TEST(StartTimeTest, fromStringInvalid) {
	StartTime startTime;
	EXPECT_THROW(startTime.fromString("abc"), invalid_argument);
	EXPECT_THROW(startTime.fromString("123 "), invalid_argument);
	EXPECT_THROW(startTime.fromString("1234a"), invalid_argument);
	EXPECT_THROW(startTime.fromString("a1234"), invalid_argument);
	EXPECT_THROW(startTime.fromString("12 34"), invalid_argument);
}

TEST(StartTimeTest, fromStringOutOfRange) {
	StartTime startTime;
	EXPECT_NO_THROW(startTime.fromString("86400"));
	EXPECT_THROW(startTime.fromString("86401"), out_of_range);
	EXPECT_THROW(startTime.fromString("4294967297"), out_of_range);
}

