#include <gmock/gmock.h>
#include <stdexcept>
#include "Logic/RunTime.h"

using namespace std;


TEST(RunTimeTest, toString) {
	RunTime runTime;
	runTime.setValue(2465);
	EXPECT_EQ("2465", runTime.toString());
}

TEST(RunTimeTest, fromString) {
	RunTime runTime;
	runTime.fromString("2465");
	EXPECT_EQ(2465, runTime.getValue());
}

TEST(RunTimeTest, fromStringInvalid) {
	RunTime runTime;
	EXPECT_THROW(runTime.fromString("abc"), invalid_argument);
	EXPECT_THROW(runTime.fromString("123 "), invalid_argument);
	EXPECT_THROW(runTime.fromString("1234a"), invalid_argument);
	EXPECT_THROW(runTime.fromString("a1234"), invalid_argument);
	EXPECT_THROW(runTime.fromString("12 34"), invalid_argument);
}

TEST(RunTimeTest, fromStringOutOfRange) {
	RunTime runTime;
	EXPECT_NO_THROW(runTime.fromString("7200"));
	EXPECT_THROW(runTime.fromString("7201"), out_of_range);
	EXPECT_THROW(runTime.fromString("4294967297"), out_of_range);
}

