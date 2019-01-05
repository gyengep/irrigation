#include <gmock/gmock.h>
#include "Exceptions/Exceptions.h"
#include "Logic/RunTime.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeTest, defaultConstructor) {
	RunTime runTime;
	EXPECT_THAT(runTime.getSeconds(), Eq(0));
}

TEST(RunTimeTest, parametrizedConstructor) {
	RunTime runTime(10);
	EXPECT_THAT(runTime.getSeconds(), Eq(10));
}

TEST(RunTimeTest, copyConstructor) {
	RunTime runTime(RunTime(20));
	EXPECT_THAT(runTime.getSeconds(), Eq(20));
}

TEST(RunTimeTest, copyOperator) {
	RunTime source(20);
	RunTime target;
	target = source;
	EXPECT_THAT(target.getSeconds(), Eq(20));
}

TEST(RunTimeTest, equalsOperator) {
	EXPECT_TRUE(RunTime(10) == RunTime(10));
	EXPECT_FALSE(RunTime(10) == RunTime(11));
	EXPECT_FALSE(RunTime(11) == RunTime(10));
}

TEST(RunTimeTest, setValue) {
	RunTime runTime;
	runTime.setSeconds(25);
	EXPECT_THAT(runTime.getSeconds(), Eq(25));
}

TEST(RunTimeTest, setValueMax) {
	RunTime runTime;
	EXPECT_NO_THROW(runTime.setSeconds(24 * 60 * 60));
	EXPECT_THROW(runTime.setSeconds(24 * 60 * 60 + 1), ValueOutOfBoundsException);
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
