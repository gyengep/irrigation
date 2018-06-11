#include <gmock/gmock.h>
#include <stdexcept>
#include "Logic/RunTime.h"

using namespace std;
using namespace testing;



TEST(RunTimeTest, init) {
	RunTime runTime;
	EXPECT_EQ(0, runTime.getValue());
}

TEST(RunTimeTest, setValue) {
	RunTime runTime;
	runTime.setValue(25);
	EXPECT_EQ(25, runTime.getValue());
}

TEST(RunTimeTest, setValueMax) {
	RunTime runTime;
	EXPECT_NO_THROW(runTime.setValue(3600 * 24));
	EXPECT_THROW(runTime.setValue(3600 * 24 + 1), out_of_range);
}

TEST(RunTimeTest, convertRunTimeDTO) {
	const RunTimeDTO expectedRunTimeDTO(50);

	RunTime runTime;
	runTime.updateFromDTO(expectedRunTimeDTO);

	EXPECT_THAT(runTime.getValue(), Eq(50));
	EXPECT_THAT(runTime.getRunTimeDTO(), Eq(expectedRunTimeDTO));
}

TEST(RunTimeTest, updateValueFromDTO) {
	RunTime runTime;
	runTime.setValue(100);

	runTime.updateFromDTO(RunTimeDTO());
	EXPECT_THAT(runTime.getValue(), Eq(100));

	runTime.updateFromDTO(RunTimeDTO().setValue(50));
	EXPECT_THAT(runTime.getValue(), Eq(50));
}
