#include <gmock/gmock.h>
#include "Hardware/Temperature/TemperatureForecast.h"

using namespace std;
using namespace testing;


TEST(TemperatureForecastTest, validateTimeString) {
	EXPECT_NO_THROW(TemperatureForecast::parseTimeString("2019-06-18T15:53:00"));
	EXPECT_ANY_THROW(TemperatureForecast::parseTimeString("2A19-06-17T18:00:00"));
	EXPECT_ANY_THROW(TemperatureForecast::parseTimeString("219-06-17T18:00:00"));
	EXPECT_ANY_THROW(TemperatureForecast::parseTimeString("A1c"));
	EXPECT_ANY_THROW(TemperatureForecast::parseTimeString("1"));
	EXPECT_ANY_THROW(TemperatureForecast::parseTimeString("456"));
	EXPECT_ANY_THROW(TemperatureForecast::parseTimeString("ABC"));
	EXPECT_ANY_THROW(TemperatureForecast::parseTimeString("abcd"));
}

TEST(TemperatureForecastTest, parseTimeString) {
	EXPECT_THAT(TemperatureForecast::parseTimeString("2019-06-18T15:54:08"), Eq(1560873248));
	EXPECT_THAT(TemperatureForecast::parseTimeString("2019-01-10T13:14:15"), Eq(1547126055));
}

TEST(TemperatureForecastTest, addTemperature) {
	TemperatureForecast temperatureForecast;

	temperatureForecast.addTemperature(10, 20, TemperatureForecast::Values(12, 14));
	temperatureForecast.addTemperature(20, 30, TemperatureForecast::Values(16, 18));
	temperatureForecast.addTemperature(30, 40, TemperatureForecast::Values(22, 28));

	EXPECT_THAT(temperatureForecast.getContainer(), ContainerEq(list<TemperatureForecast::ValuesWithTimes>{
		TemperatureForecast::ValuesWithTimes(10, 20, TemperatureForecast::Values(12, 14)),
		TemperatureForecast::ValuesWithTimes(20, 30, TemperatureForecast::Values(16, 18)),
		TemperatureForecast::ValuesWithTimes(30, 40, TemperatureForecast::Values(22, 28))
	}));
}

TEST(TemperatureForecastTest, addTemperatureInvalid) {
	TemperatureForecast temperatureForecast;

	EXPECT_ANY_THROW(temperatureForecast.addTemperature(30, 30, TemperatureForecast::Values(12, 14)));
	EXPECT_ANY_THROW(temperatureForecast.addTemperature(20, 10, TemperatureForecast::Values(12, 14)));
}

TEST(TemperatureForecastTest, addTemperatureMismatch) {
	TemperatureForecast temperatureForecast;

	EXPECT_NO_THROW(temperatureForecast.addTemperature(10, 20, TemperatureForecast::Values(12, 14)));
	EXPECT_ANY_THROW(temperatureForecast.addTemperature(19, 30, TemperatureForecast::Values(12, 14)));
	EXPECT_ANY_THROW(temperatureForecast.addTemperature(21, 30, TemperatureForecast::Values(12, 14)));
}

TEST(TemperatureForecastTest, getForecast) {
	TemperatureForecast temperatureForecast;

	temperatureForecast.addTemperature(10, 20, TemperatureForecast::Values(12, 14));
	temperatureForecast.addTemperature(20, 30, TemperatureForecast::Values(16, 18));
	temperatureForecast.addTemperature(30, 40, TemperatureForecast::Values(20, 22));
	temperatureForecast.addTemperature(40, 50, TemperatureForecast::Values(24, 26));
	temperatureForecast.addTemperature(50, 60, TemperatureForecast::Values(28, 30));

	EXPECT_THAT(temperatureForecast.getForecast(20, 40), TemperatureForecast::Values(16, 22));
	EXPECT_THAT(temperatureForecast.getForecast(15, 40), TemperatureForecast::Values(12, 22));
	EXPECT_THAT(temperatureForecast.getForecast(15, 45), TemperatureForecast::Values(12, 26));
	EXPECT_THAT(temperatureForecast.getForecast(20, 45), TemperatureForecast::Values(16, 26));
}

TEST(TemperatureForecastTest, getForecastOutOfBounds) {
	TemperatureForecast temperatureForecast;

	temperatureForecast.addTemperature(10, 20, TemperatureForecast::Values(12, 14));
	temperatureForecast.addTemperature(20, 30, TemperatureForecast::Values(16, 18));
	temperatureForecast.addTemperature(30, 40, TemperatureForecast::Values(20, 22));
	temperatureForecast.addTemperature(40, 50, TemperatureForecast::Values(24, 26));
	temperatureForecast.addTemperature(50, 60, TemperatureForecast::Values(28, 30));

	EXPECT_ANY_THROW(temperatureForecast.getForecast(60, 70));
	EXPECT_ANY_THROW(temperatureForecast.getForecast(5, 10));
}
