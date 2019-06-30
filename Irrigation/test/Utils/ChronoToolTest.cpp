#include <gmock/gmock.h>
#include <chrono>
#include "Views/TimerView/ChronoTools.h"

using namespace std;
using namespace std::chrono;


TEST(TimeConverterTest, getMillis) {
	const milliseconds ms(240);

	TimeConverter timeConverter(ms);
	EXPECT_EQ(0, timeConverter.getDays());
	EXPECT_EQ(0, timeConverter.getHours());
	EXPECT_EQ(0, timeConverter.getMinutes());
	EXPECT_EQ(0, timeConverter.getSeconds());
	EXPECT_EQ(240, timeConverter.getMillis());
}

TEST(TimeConverterTest, getSecond) {
	const milliseconds ms((2 * 1000) + 240);

	TimeConverter timeConverter(ms);
	EXPECT_EQ(0, timeConverter.getDays());
	EXPECT_EQ(0, timeConverter.getHours());
	EXPECT_EQ(0, timeConverter.getMinutes());
	EXPECT_EQ(2, timeConverter.getSeconds());
	EXPECT_EQ(240, timeConverter.getMillis());
}

TEST(TimeConverterTest, getMinutes) {
	const milliseconds ms(
			(7 * 60 * 1000) +
			(32 * 1000) +
			54);

	TimeConverter timeConverter(ms);
	EXPECT_EQ(0, timeConverter.getDays());
	EXPECT_EQ(0, timeConverter.getHours());
	EXPECT_EQ(7, timeConverter.getMinutes());
	EXPECT_EQ(32, timeConverter.getSeconds());
	EXPECT_EQ(54, timeConverter.getMillis());
}

TEST(TimeConverterTest, getHours) {
	const milliseconds ms(
			(17 * 60 * 60 * 1000) +
			(12 * 60 * 1000) +
			(46 * 1000) +
			54);

	TimeConverter timeConverter(ms);
	EXPECT_EQ(0, timeConverter.getDays());
	EXPECT_EQ(17, timeConverter.getHours());
	EXPECT_EQ(12, timeConverter.getMinutes());
	EXPECT_EQ(46, timeConverter.getSeconds());
	EXPECT_EQ(54, timeConverter.getMillis());
}

TEST(TimeConverterTest, getDays) {
	const milliseconds ms(
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
	EXPECT_EQ(54, timeConverter.getMillis());
}

TEST(TimeConverterTest, absSeconds) {
	EXPECT_EQ(seconds(5), abs(seconds(5)));
	EXPECT_EQ(seconds(5), abs(seconds(-5)));
	EXPECT_EQ(seconds(5000000), abs(seconds(5000000)));
	EXPECT_EQ(seconds(5000000), abs(seconds(-5000000)));
}

TEST(TimeConverterTest, absMilliseconds) {
	EXPECT_EQ(milliseconds(8), abs(milliseconds(8)));
	EXPECT_EQ(milliseconds(8), abs(milliseconds(-8)));
	EXPECT_EQ(milliseconds(80000000000), abs(milliseconds(80000000000)));
	EXPECT_EQ(milliseconds(80000000000), abs(milliseconds(-80000000000)));
}
