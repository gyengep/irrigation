#include <gmock/gmock.h>
#include "Utils/TimeConversion.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(TimeConverterTest, getMillis) {
	TimeConverter timeConverter(
			chrono::milliseconds(240)
		);

	EXPECT_THAT(timeConverter.getDays(), Eq(0));
	EXPECT_THAT(timeConverter.getHours(), Eq(0));
	EXPECT_THAT(timeConverter.getMinutes(), Eq(0));
	EXPECT_THAT(timeConverter.getSeconds(), Eq(0));
	EXPECT_THAT(timeConverter.getMillis(), Eq(240));
}

TEST(TimeConverterTest, getSecond) {
	TimeConverter timeConverter(
			chrono::seconds(2) + chrono::milliseconds(240)
		);

	EXPECT_THAT(timeConverter.getDays(), Eq(0));
	EXPECT_THAT(timeConverter.getHours(), Eq(0));
	EXPECT_THAT(timeConverter.getMinutes(), Eq(0));
	EXPECT_THAT(timeConverter.getSeconds(), Eq(2));
	EXPECT_THAT(timeConverter.getMillis(), Eq(240));
}

TEST(TimeConverterTest, getMinutes) {
	TimeConverter timeConverter(
			chrono::minutes(7) + chrono::seconds(32) + chrono::milliseconds(54)
		);

	EXPECT_THAT(timeConverter.getDays(), Eq(0));
	EXPECT_THAT(timeConverter.getHours(), Eq(0));
	EXPECT_THAT(timeConverter.getMinutes(), Eq(7));
	EXPECT_THAT(timeConverter.getSeconds(), Eq(32));
	EXPECT_THAT(timeConverter.getMillis(), Eq(54));
}

TEST(TimeConverterTest, getHours) {
	TimeConverter timeConverter(
			chrono::hours(17) + chrono::minutes(12) + chrono::seconds(46) +
			chrono::milliseconds(54)
		);

	EXPECT_THAT(timeConverter.getDays(), Eq(0));
	EXPECT_THAT(timeConverter.getHours(), Eq(17));
	EXPECT_THAT(timeConverter.getMinutes(), Eq(12));
	EXPECT_THAT(timeConverter.getSeconds(), Eq(46));
	EXPECT_THAT(timeConverter.getMillis(), Eq(54));
}

TEST(TimeConverterTest, getDays) {
	TimeConverter timeConverter(
			chrono::hours(5) * 24 + chrono::hours(17) + chrono::minutes(12) +
			chrono::seconds(46) + chrono::milliseconds(54)
		);

	EXPECT_THAT(timeConverter.getDays(), Eq(5));
	EXPECT_THAT(timeConverter.getHours(), Eq(17));
	EXPECT_THAT(timeConverter.getMinutes(), Eq(12));
	EXPECT_THAT(timeConverter.getSeconds(), Eq(46));
	EXPECT_THAT(timeConverter.getMillis(), Eq(54));
}
