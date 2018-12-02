#include <gmock/gmock.h>
#include "Exceptions/Exceptions.h"
#include "Logic/StartTime.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeTest, defaultConstructor) {
	StartTime startTime;
	EXPECT_EQ(0, startTime.getHours());
	EXPECT_EQ(0, startTime.getMinutes());
}

TEST(StartTimeTest, parametrizedConstructor) {
	StartTime startTime(1, 30);
	EXPECT_EQ(1, startTime.getHours());
	EXPECT_EQ(30, startTime.getMinutes());
}

TEST(StartTimeTest, equalOperator) {
	StartTime startTime;

	EXPECT_TRUE(StartTime() == startTime);

	const unsigned hour = 15;
	const unsigned minute = 25;

	startTime.set(hour, minute);
	EXPECT_FALSE(StartTime() == startTime);
	EXPECT_TRUE(StartTime(hour, minute) == startTime);
	EXPECT_FALSE(StartTime(hour, minute + 1) == startTime);
	EXPECT_FALSE(StartTime(hour, minute - 1) == startTime);
	EXPECT_FALSE(StartTime(hour + 1, minute) == startTime);
	EXPECT_FALSE(StartTime(hour - 1, minute) == startTime);
}

TEST(StartTimeTest, equals) {
	const unsigned hour = 15;
	const unsigned minute = 25;

	StartTime startTime(hour, minute);

	EXPECT_TRUE(startTime.equals(hour, minute, 0));

	EXPECT_FALSE(startTime.equals(hour, minute, 59));
	EXPECT_FALSE(startTime.equals(hour, minute, 1));
	EXPECT_FALSE(startTime.equals(hour, minute - 1, 0));
	EXPECT_FALSE(startTime.equals(hour, minute + 1, 0));
	EXPECT_FALSE(startTime.equals(hour - 1, minute, 0));
	EXPECT_FALSE(startTime.equals(hour + 1, minute, 0));
}

TEST(StartTimeTest, lessOperator) {

	const unsigned hour = 15;
	const unsigned minute = 25;

	StartTime startTime(hour, minute);

	EXPECT_TRUE(StartTime(hour - 1, minute - 1) < startTime);
	EXPECT_TRUE(StartTime(hour - 1, minute) < startTime);
	EXPECT_TRUE(StartTime(hour - 1, minute + 1) < startTime);

	EXPECT_TRUE(StartTime(hour, minute - 1) < startTime);
	EXPECT_FALSE(StartTime(hour, minute) < startTime);
	EXPECT_FALSE(StartTime(hour, minute + 1) < startTime);

	EXPECT_FALSE(StartTime(hour + 1, minute - 1) < startTime);
	EXPECT_FALSE(StartTime(hour + 1, minute) < startTime);
	EXPECT_FALSE(StartTime(hour + 1, minute + 1) < startTime);
}

TEST(StartTimeTest, set) {
	StartTime startTime;

	startTime.set(21, 36);
	EXPECT_EQ(21, startTime.getHours());
	EXPECT_EQ(36, startTime.getMinutes());
}

TEST(StartTimeTest, setValueMax) {
	StartTime startTime;
	EXPECT_NO_THROW(StartTime(23, 59));
	EXPECT_THROW(StartTime(24, 0), ValueOutOfBoundsException);
	EXPECT_THROW(StartTime(23, 60), ValueOutOfBoundsException);
}

TEST(StartTimeTest, convertStartTimeDTO) {
	const StartTimeDTO expectedStartTimeDTO(20, 12);

	StartTime startTime;
	startTime.updateFromDTO(expectedStartTimeDTO);

	EXPECT_THAT(startTime.getHours(), Eq(20));
	EXPECT_THAT(startTime.getMinutes(), Eq(12));
	EXPECT_THAT(startTime.getStartTimeDTO(), Eq(expectedStartTimeDTO));
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
