#include <gmock/gmock.h>
#include <chrono>
#include "Views/TimerView/ChronoTools.h"


using namespace std;

TEST(TimeConverterTest, getSecond) {
	const chrono::milliseconds ms(
			(2 * 1000) +
			24);

	TimeConverter timeConverter(ms);
	EXPECT_EQ(0, timeConverter.getDays());
	EXPECT_EQ(0, timeConverter.getHours());
	EXPECT_EQ(0, timeConverter.getMinutes());
	EXPECT_EQ(2, timeConverter.getSeconds());
}

TEST(TimeConverterTest, getMinutes) {
	const chrono::milliseconds ms(
			(7 * 60 * 1000) +
			(32 * 1000) +
			54);

	TimeConverter timeConverter(ms);
	EXPECT_EQ(0, timeConverter.getDays());
	EXPECT_EQ(0, timeConverter.getHours());
	EXPECT_EQ(7, timeConverter.getMinutes());
	EXPECT_EQ(32, timeConverter.getSeconds());
}

TEST(TimeConverterTest, getHours) {
	const chrono::milliseconds ms(
			(17 * 60 * 60 * 1000) +
			(12 * 60 * 1000) +
			(46 * 1000) +
			54);

	TimeConverter timeConverter(ms);
	EXPECT_EQ(0, timeConverter.getDays());
	EXPECT_EQ(17, timeConverter.getHours());
	EXPECT_EQ(12, timeConverter.getMinutes());
	EXPECT_EQ(46, timeConverter.getSeconds());
}

TEST(TimeConverterTest, getDays) {
	const chrono::milliseconds ms(
			(5 * 24 * 60 * 60 * 1000) +
			(6 * 60 * 60 * 1000) +
			(7 * 60 * 1000) +
			(8 * 1000) +
			54);

	TimeConverter timeConverter(ms);
	EXPECT_EQ(5, timeConverter.getDays());
	EXPECT_EQ(6, timeConverter.getHours());
	EXPECT_EQ(7, timeConverter.getMinutes());
	EXPECT_EQ(8, timeConverter.getSeconds());
}
