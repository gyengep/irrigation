#include <gmock/gmock.h>
#include "Schedulers/TemperatureToPercent.h"

using namespace std;
using namespace testing;


TEST(TemperatureToPercentTest, getRequiredPercentFromTemperature) {
	EXPECT_THAT(TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(15.0f), Eq(25));
	EXPECT_THAT(TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(16.0f), Eq(27));
	EXPECT_THAT(TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(17.0f), Eq(30));
	EXPECT_THAT(TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(18.0f), Eq(32));
	EXPECT_THAT(TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(19.0f), Eq(35));
	EXPECT_THAT(TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(20.0f), Eq(37));
	EXPECT_THAT(TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(21.0f), Eq(40));
	EXPECT_THAT(TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(22.0f), Eq(42));
	EXPECT_THAT(TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(23.0f), Eq(45));
	EXPECT_THAT(TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(24.0f), Eq(47));
	EXPECT_THAT(TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(25.0f), Eq(50));
	EXPECT_THAT(TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(26.0f), Eq(55));
	EXPECT_THAT(TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(27.0f), Eq(60));
	EXPECT_THAT(TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(28.0f), Eq(65));
	EXPECT_THAT(TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(29.0f), Eq(70));
	EXPECT_THAT(TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(30.0f), Eq(75));
	EXPECT_THAT(TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(31.0f), Eq(80));
	EXPECT_THAT(TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(32.0f), Eq(85));
	EXPECT_THAT(TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(33.0f), Eq(90));
	EXPECT_THAT(TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(34.0f), Eq(95));
	EXPECT_THAT(TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(35.0f), Eq(100));
}

TEST(TemperatureToPercentTest, getRequiredPercentFromTemperatureLower) {
	EXPECT_THAT(TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(12.0f), Eq(0));
	EXPECT_THAT(TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(13.0f), Eq(0));
	EXPECT_THAT(TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(14.0f), Eq(0));
}

TEST(TemperatureToPercentTest, getRequiredPercentFromTemperatureHigher) {
	EXPECT_THAT(TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(36.0f), Eq(105));
	EXPECT_THAT(TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(37.0f), Eq(110));
	EXPECT_THAT(TemperatureToPercent::getInstance().getRequiredPercentFromTemperature(38.0f), Eq(115));
}

