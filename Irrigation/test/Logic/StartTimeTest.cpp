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

TEST(StartTimeTest, getStartTimeDTO) {
	StartTime startTime;
	startTime.setValue(50);

	StartTimeDTO startTimeDTO = startTime.getStartTimeDTO();

	EXPECT_FALSE(startTimeDTO.hasId());
	EXPECT_TRUE(startTimeDTO.hasValue());

	EXPECT_EQ(50, startTimeDTO.getValue());
}
