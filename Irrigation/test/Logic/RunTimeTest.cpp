#include <gmock/gmock.h>
#include <stdexcept>
#include "Logic/RunTime.h"

using namespace std;



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

TEST(RunTimeTest, getRunTimeDTO) {
	RunTime runTime;
	runTime.setValue(50);

	const RunTimeDTO expectedRunTimeDTO(50);

	EXPECT_EQ(expectedRunTimeDTO, runTime.getRunTimeDTO());
}
