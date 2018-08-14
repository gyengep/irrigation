#include <gmock/gmock.h>
#include "Exceptions/Exceptions.h"
#include "Logic/StartTime.h"

using namespace std;
using namespace testing;



TEST(StartTimeTest, init) {
	StartTime startTime;
	EXPECT_EQ(0, startTime.getHours());
	EXPECT_EQ(0, startTime.getMinutes());
	EXPECT_EQ(0, startTime.getSeconds());
}

TEST(StartTimeTest, initWithHourMinSec) {
	StartTime startTime(1, 30, 10);
	EXPECT_EQ(1, startTime.getHours());
	EXPECT_EQ(30, startTime.getMinutes());
	EXPECT_EQ(10, startTime.getSeconds());
}

TEST(StartTimeTest, initWithHourMin) {
	StartTime startTime(1, 30);
	EXPECT_EQ(1, startTime.getHours());
	EXPECT_EQ(30, startTime.getMinutes());
	EXPECT_EQ(0, startTime.getSeconds());
}

TEST(StartTimeTest, set) {
	StartTime startTime;

	startTime.set(21, 36, 35);
	EXPECT_EQ(21, startTime.getHours());
	EXPECT_EQ(36, startTime.getMinutes());
	EXPECT_EQ(35, startTime.getSeconds());

	startTime.set(22, 37);
	EXPECT_EQ(22, startTime.getHours());
	EXPECT_EQ(37, startTime.getMinutes());
	EXPECT_EQ(0, startTime.getSeconds());
}

TEST(StartTimeTest, setValueMax) {
	StartTime startTime;
	EXPECT_NO_THROW(StartTime(23, 59, 59));
	EXPECT_THROW(StartTime(24, 0, 0), ValueOutOfBoundsException);
	EXPECT_THROW(StartTime(23, 60, 0), ValueOutOfBoundsException);
	EXPECT_THROW(StartTime(23, 59, 60), ValueOutOfBoundsException);
}

TEST(StartTimeTest, equal) {
	StartTime startTime;

	EXPECT_TRUE(StartTime() == startTime);

	startTime.set(15, 25, 5);
	EXPECT_FALSE(StartTime() == startTime);
	EXPECT_TRUE(StartTime(15, 25, 5) == startTime);
	EXPECT_FALSE(StartTime(15, 25, 4) == startTime);
	EXPECT_FALSE(StartTime(15, 25, 6) == startTime);
	EXPECT_FALSE(StartTime(15, 24, 5) == startTime);
	EXPECT_FALSE(StartTime(15, 26, 5) == startTime);
	EXPECT_FALSE(StartTime(14, 25, 5) == startTime);
	EXPECT_FALSE(StartTime(16, 25, 5) == startTime);
}

TEST(StartTimeTest, less) {
	StartTime startTime(15, 25, 5);

	EXPECT_FALSE(StartTime(15, 25, 5) < startTime);

	EXPECT_TRUE(StartTime(15, 25, 4) < startTime);
	EXPECT_FALSE(StartTime(15, 25, 5) < startTime);
	EXPECT_FALSE(StartTime(15, 25, 6) < startTime);

	EXPECT_TRUE(StartTime(15, 24, 4) < startTime);
	EXPECT_TRUE(StartTime(15, 24, 5) < startTime);
	EXPECT_TRUE(StartTime(15, 24, 6) < startTime);
	EXPECT_FALSE(StartTime(15, 26, 4) < startTime);
	EXPECT_FALSE(StartTime(15, 26, 5) < startTime);
	EXPECT_FALSE(StartTime(15, 26, 6) < startTime);

	EXPECT_TRUE(StartTime(14, 24, 4) < startTime);
	EXPECT_TRUE(StartTime(14, 24, 5) < startTime);
	EXPECT_TRUE(StartTime(14, 24, 6) < startTime);
	EXPECT_TRUE(StartTime(14, 25, 4) < startTime);
	EXPECT_TRUE(StartTime(14, 25, 5) < startTime);
	EXPECT_TRUE(StartTime(14, 25, 6) < startTime);
	EXPECT_TRUE(StartTime(14, 26, 4) < startTime);
	EXPECT_TRUE(StartTime(14, 26, 5) < startTime);
	EXPECT_TRUE(StartTime(14, 26, 6) < startTime);
	EXPECT_FALSE(StartTime(16, 24, 4) < startTime);
	EXPECT_FALSE(StartTime(16, 24, 5) < startTime);
	EXPECT_FALSE(StartTime(16, 24, 6) < startTime);
	EXPECT_FALSE(StartTime(16, 25, 4) < startTime);
	EXPECT_FALSE(StartTime(16, 25, 5) < startTime);
	EXPECT_FALSE(StartTime(16, 25, 6) < startTime);
	EXPECT_FALSE(StartTime(16, 26, 4) < startTime);
	EXPECT_FALSE(StartTime(16, 26, 5) < startTime);
	EXPECT_FALSE(StartTime(16, 26, 6) < startTime);
}

TEST(StartTimeTest, convertStartTimeDTO) {
	const StartTimeDTO expectedStartTimeDTO(20, 12);

	StartTime startTime;
	startTime.updateFromDTO(expectedStartTimeDTO);

	EXPECT_THAT(startTime.getHours(), Eq(20));
	EXPECT_THAT(startTime.getMinutes(), Eq(12));
	EXPECT_THAT(startTime.getStartTimeDTO(), Eq(expectedStartTimeDTO));
}

TEST(StartTimeTest, getStartTimeDTO) {
	StartTime startTime;

	startTime.set(1, 2);
	EXPECT_NO_THROW(startTime.getStartTimeDTO());

	startTime.set(1, 2, 3);
	EXPECT_THROW(startTime.getStartTimeDTO(), logic_error);
}

TEST(StartTimeTest, updateHourAndMinuteFromDTO) {
	StartTime startTime;
	startTime.set(10, 9);

	startTime.updateFromDTO(StartTimeDTO());
	EXPECT_THAT(startTime.getHours(), Eq(10));
	EXPECT_THAT(startTime.getMinutes(), Eq(9));

	startTime.updateFromDTO(StartTimeDTO().setHour(15).setMinute(21));
	EXPECT_THAT(startTime.getHours(), Eq(15));
	EXPECT_THAT(startTime.getMinutes(), Eq(21));
}
