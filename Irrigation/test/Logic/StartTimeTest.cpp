#include <gmock/gmock.h>
#include <stdexcept>
#include "Logic/StartTime.h"

using namespace std;
using namespace testing;



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

TEST(StartTimeTest, setValueMax) {
	StartTime startTime;
	EXPECT_NO_THROW(StartTime(23, 59, 59));
	EXPECT_THROW(StartTime(24, 0, 0), out_of_range);
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

TEST(StartTimeTest, less) {
	StartTime startTime(1, 0, 1);

	EXPECT_TRUE(StartTime(1, 0, 0) < startTime);
	EXPECT_FALSE(StartTime(1, 0, 1) < startTime);
	EXPECT_FALSE(StartTime(1, 0, 2) < startTime);
}

TEST(StartTimeTest, convertStartTimeDTO) {
	const StartTimeDTO expectedStartTimeDTO(50);

	StartTime startTime;
	startTime.updateFromDTO(expectedStartTimeDTO);

	EXPECT_THAT(startTime.getValue(), Eq(50));
	EXPECT_THAT(startTime.getStartTimeDTO(), Eq(expectedStartTimeDTO));
}

TEST(StartTimeTest, updateValueFromDTO) {
	StartTime startTime;
	startTime.setValue(100);

	startTime.updateFromDTO(StartTimeDTO());
	EXPECT_THAT(startTime.getValue(), Eq(100));

	startTime.updateFromDTO(StartTimeDTO().setValue(50));
	EXPECT_THAT(startTime.getValue(), Eq(50));
}
