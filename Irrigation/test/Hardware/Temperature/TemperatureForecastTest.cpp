#include <gmock/gmock.h>
#include "Hardware/Temperature/TemperatureForecast.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

class MockTemperatureForecastReader : public TemperatureForecast::NetworkReader {
public:
	MOCK_CONST_METHOD3(read, string(const string& url, const string& location, const string& appid));
};

///////////////////////////////////////////////////////////////////////////////

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
	TemperatureForecast temperatureForecast(chrono::minutes(1));

	temperatureForecast.addTemperature(10, 20, TemperatureForecast::MinMaxValues(12, 14));
	temperatureForecast.addTemperature(20, 30, TemperatureForecast::MinMaxValues(16, 18));
	temperatureForecast.addTemperature(30, 40, TemperatureForecast::MinMaxValues(22, 28));

	EXPECT_THAT(temperatureForecast.getContainer(), ContainerEq(list<TemperatureForecast::MinMaxValuesWithTimes>{
		TemperatureForecast::MinMaxValuesWithTimes(10, 20, TemperatureForecast::MinMaxValues(12, 14)),
		TemperatureForecast::MinMaxValuesWithTimes(20, 30, TemperatureForecast::MinMaxValues(16, 18)),
		TemperatureForecast::MinMaxValuesWithTimes(30, 40, TemperatureForecast::MinMaxValues(22, 28))
	}));
}

TEST(TemperatureForecastTest, addTemperatureInvalid) {
	TemperatureForecast temperatureForecast(chrono::minutes(1));

	EXPECT_ANY_THROW(temperatureForecast.addTemperature(30, 30, TemperatureForecast::MinMaxValues(12, 14)));
	EXPECT_ANY_THROW(temperatureForecast.addTemperature(20, 10, TemperatureForecast::MinMaxValues(12, 14)));
}

TEST(TemperatureForecastTest, addTemperatureMismatch) {
	TemperatureForecast temperatureForecast(chrono::minutes(1));

	EXPECT_NO_THROW(temperatureForecast.addTemperature(10, 20, TemperatureForecast::MinMaxValues(12, 14)));
	EXPECT_ANY_THROW(temperatureForecast.addTemperature(19, 30, TemperatureForecast::MinMaxValues(12, 14)));
	EXPECT_ANY_THROW(temperatureForecast.addTemperature(21, 30, TemperatureForecast::MinMaxValues(12, 14)));
}

TEST(TemperatureForecastTest, getForecast) {
	TemperatureForecast temperatureForecast(chrono::minutes(1));

	temperatureForecast.addTemperature(10, 20, TemperatureForecast::MinMaxValues(12, 14));
	temperatureForecast.addTemperature(20, 30, TemperatureForecast::MinMaxValues(16, 18));
	temperatureForecast.addTemperature(30, 40, TemperatureForecast::MinMaxValues(20, 22));
	temperatureForecast.addTemperature(40, 50, TemperatureForecast::MinMaxValues(24, 26));
	temperatureForecast.addTemperature(50, 60, TemperatureForecast::MinMaxValues(28, 30));

	EXPECT_THAT(temperatureForecast.getForecast(20, 40), TemperatureForecast::MinMaxValues(16, 22));
	EXPECT_THAT(temperatureForecast.getForecast(15, 40), TemperatureForecast::MinMaxValues(12, 22));
	EXPECT_THAT(temperatureForecast.getForecast(15, 45), TemperatureForecast::MinMaxValues(12, 26));
	EXPECT_THAT(temperatureForecast.getForecast(20, 45), TemperatureForecast::MinMaxValues(16, 26));
}

TEST(TemperatureForecastTest, getForecastOutOfBounds) {
	TemperatureForecast temperatureForecast(chrono::minutes(1));

	temperatureForecast.addTemperature(10, 20, TemperatureForecast::MinMaxValues(12, 14));
	temperatureForecast.addTemperature(20, 30, TemperatureForecast::MinMaxValues(16, 18));
	temperatureForecast.addTemperature(30, 40, TemperatureForecast::MinMaxValues(20, 22));
	temperatureForecast.addTemperature(40, 50, TemperatureForecast::MinMaxValues(24, 26));
	temperatureForecast.addTemperature(50, 60, TemperatureForecast::MinMaxValues(28, 30));

	EXPECT_ANY_THROW(temperatureForecast.getForecast(60, 70));
	EXPECT_ANY_THROW(temperatureForecast.getForecast(5, 10));
}

const string expectedXml1 =
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
		"<weatherdata>"
			"<forecast>"
				"<time from= \"2019-06-20T18:00:00\" to=\"2019-06-20T21:00:00\">"
					"<temperature unit=\"celsius\" value=\"17.84\" min=\"17.84\" max=\"18.6\"></temperature>"
				"</time>"
				"<time from=\"2019-06-20T21:00:00\" to=\"2019-06-21T00:00:00\">"
					"<temperature unit=\"celsius\" value=\"16.86\" min=\"16.86\" max=\"17.43\"></temperature>"
				"</time>"
			"</forecast>"
		"</weatherdata>";


const list<TemperatureForecast::MinMaxValuesWithTimes> expectedList1 {
	TemperatureForecast::MinMaxValuesWithTimes(1561053600, 1561064400, TemperatureForecast::MinMaxValues(17.84, 18.6)),
	TemperatureForecast::MinMaxValuesWithTimes(1561064400, 1561075200, TemperatureForecast::MinMaxValues(16.86, 17.43))
};


TEST(TemperatureForecastTest, parseXml) {
	EXPECT_THAT(TemperatureForecast::parseXml(expectedXml1), ContainerEq(expectedList1));
}

TEST(TemperatureForecastTest, parseXmlInvalid) {
	EXPECT_ANY_THROW(TemperatureForecast::parseXml("abc"));
}

TEST(TemperatureForecastTest, parseXmlEmpty) {
	EXPECT_THAT(TemperatureForecast::parseXml("<?xml version=\"1.0\" encoding=\"UTF-8\"?><weatherdata></weatherdata>"), IsEmpty());
}

TEST(TemperatureForecastTest, updateCache) {

	auto mockTemperatureForecastReader = make_shared<MockTemperatureForecastReader>();

	EXPECT_CALL(*mockTemperatureForecastReader, read(_, _, _)).Times(1).WillOnce(Return(expectedXml1));

	TemperatureForecast temperatureForecast(chrono::minutes(1), mockTemperatureForecastReader);
	temperatureForecast.updateCache();

	EXPECT_THAT(temperatureForecast.getContainer(), ContainerEq(expectedList1));
}

TEST(TemperatureForecastTest, updateCacheEmpty) {
	auto mockTemperatureForecastReader = make_shared<MockTemperatureForecastReader>();

	EXPECT_CALL(*mockTemperatureForecastReader, read(_, _, _)).
			Times(1).
			WillOnce(Return("<?xml version=\"1.0\" encoding=\"UTF-8\"?><weatherdata></weatherdata>"));

	TemperatureForecast temperatureForecast(chrono::minutes(1), mockTemperatureForecastReader);
	temperatureForecast.updateCache();

	EXPECT_THAT(temperatureForecast.getContainer(), IsEmpty());
}

TEST(TemperatureForecastTest, updateCacheInvalid) {
	auto mockTemperatureForecastReader = make_shared<MockTemperatureForecastReader>();

	EXPECT_CALL(*mockTemperatureForecastReader, read(_, _, _)).Times(1).WillOnce(Throw(exception()));

	TemperatureForecast temperatureForecast(chrono::minutes(1), mockTemperatureForecastReader);
	temperatureForecast.updateCache();

	EXPECT_THAT(temperatureForecast.getContainer(), IsEmpty());
}
