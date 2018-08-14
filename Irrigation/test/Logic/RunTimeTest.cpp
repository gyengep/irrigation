#include <gmock/gmock.h>
#include "Exceptions/Exceptions.h"
#include "Logic/RunTime.h"

using namespace std;
using namespace testing;



TEST(RunTimeTest, init) {
	RunTime runTime;
	EXPECT_EQ(0, runTime.getSeconds());
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
