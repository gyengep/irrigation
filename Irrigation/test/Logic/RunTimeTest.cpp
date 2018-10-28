#include <gmock/gmock.h>
#include "Exceptions/Exceptions.h"
#include "Logic/RunTime.h"

using namespace std;
using namespace testing;


TEST(RunTimeTest, initDefault) {
	RunTime runTime;
	EXPECT_EQ(0, runTime.getSeconds());
}

TEST(RunTimeTest, initConversion) {
	RunTime runTime(10);
	EXPECT_EQ(10, runTime.getSeconds());
}

TEST(RunTimeTest, initCopy) {
	RunTime runTime(RunTime(20));
	EXPECT_EQ(20, runTime.getSeconds());
}

TEST(RunTimeTest, copy) {
	RunTime source(20);
	RunTime target;
	target = source;
	EXPECT_EQ(20, source.getSeconds());
}

TEST(RunTimeTest, equals) {
	EXPECT_TRUE(RunTime(10) == RunTime(10));
	EXPECT_FALSE(RunTime(10) == RunTime(11));
	EXPECT_FALSE(RunTime(11) == RunTime(10));
}

TEST(RunTimeTest, setValue) {
	RunTime runTime;
	runTime.setSeconds(25);
	EXPECT_EQ(25, runTime.getSeconds());
}

TEST(RunTimeTest, setValueMax) {
	RunTime runTime;
	EXPECT_NO_THROW(runTime.setSeconds(3600 * 24));
	EXPECT_THROW(runTime.setSeconds(3600 * 24 + 1), ValueOutOfBoundsException);
}

TEST(RunTimeTest, convertRunTimeDTO) {
	const RunTimeDTO expectedRunTimeDTO(5, 10);

	RunTime runTime;
	runTime.updateFromDTO(expectedRunTimeDTO);

	EXPECT_THAT(runTime.getSeconds(), Eq(60 * 5 + 10));
	EXPECT_THAT(runTime.getRunTimeDTO(), Eq(expectedRunTimeDTO));
}

TEST(RunTimeTest, updateSecondsFromDTO) {
	RunTime runTime;
	runTime.setSeconds(100);

	runTime.updateFromDTO(RunTimeDTO());
	EXPECT_THAT(runTime.getSeconds(), Eq(100));

	runTime.updateFromDTO(RunTimeDTO().setSeconds(50));
	EXPECT_THAT(runTime.getSeconds(), Eq(50));
}
